#include <cassert>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <string>
#include <strings.h>
#include <fstream>
#include <csignal>
#include <random>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"

constexpr int port = 8888;
constexpr int min_dc = 500000;
constexpr int max_dc = 1800000;
constexpr int pwm_period = 2000000;

int server_socket;
int client_socket;
int angle = 0;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
struct sigaction sigIntHandler;
VL53L0X_Dev_t sensor = {0};

void init_socket()
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        std::cerr << ("ERROR opening socket\n");
        exit(-1);
    }

    bzero((char *)&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        std::cerr << ("ERROR on binding\n");
        exit(-1);
    }

    listen(server_socket, 5);
    std::cout << "Waiting for connection on port " << port << "\n";

    socklen_t clilen = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &clilen);
    if (client_socket < 0)
    {
        std::cerr << ("ERROR on accept\n");
        exit(-1);
    }

    std::cout << "Accepted TCP connection from " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port)
              << "\n";
}

void send_tcp_message(const std::string &message)
{
    int result = write(client_socket, message.c_str(), message.length());
    if (result < 0)
    {
        std::cerr << ("ERROR writing to socket\n");
        exit(EXIT_FAILURE);
    }
}

void close_socket()
{
    std::cout << "Closing socket\n";
    close(client_socket);
    close(server_socket);
}

void check_measurement_data(const VL53L0X_RangingMeasurementData_t &data)
{
    if (data.RangeStatus != 0)
    {
        std::cerr << "ERROR on measuring distance\n";
        exit(EXIT_FAILURE);
    }
}

void write_to_file(const std::string &path, const std::string &text)
{
    std::ofstream file;
    file.open(path);
    file << text << "\n";
    file.close();
}

uint32_t calculate_duty_cycle(const uint32_t &angle)
{
    uint32_t duty_cycle_increase = (max_dc - min_dc) * angle / 180;
    return min_dc + duty_cycle_increase;
}

void rotate_servo(const uint32_t &angle)
{
    std::cout << "Rotating REAL servo to angle: " << angle << "\n";

    uint32_t duty_cycle = calculate_duty_cycle(angle);

    write_to_file("/sys/class/pwm/pwmchip0/pwm0/period", std::to_string(pwm_period));
    write_to_file("/sys/class/pwm/pwmchip0/pwm0/duty_cycle", std::to_string(duty_cycle));
    write_to_file("/sys/class/pwm/pwmchip0/pwm0/enable", "1");
    usleep(150000);
    write_to_file("/sys/class/pwm/pwmchip0/pwm0/enable", "0");
}

void check_status(const VL53L0X_Error &sensor_status)
{
    if (sensor_status != VL53L0X_ERROR_NONE)
    {
        std::cerr << "SENSOR ERROR\n";
        exit(EXIT_FAILURE);
    }
}

uint16_t measure_distance(VL53L0X_Dev_t &sensor)
{
    VL53L0X_RangingMeasurementData_t measurement_data;
    VL53L0X_Error status = VL53L0X_PerformSingleRangingMeasurement(&sensor, &measurement_data);
    check_status(status);
    check_measurement_data(measurement_data);
    return measurement_data.RangeMilliMeter;
}

void init_sensor(VL53L0X_Dev_t &sensor)
{
    std::cout << "Initializing REAL VL53L0X sensor\n";

    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    VL53L0X_Error sensor_status;

    sensor.I2cDevAddr = 0x29;
    char *i2c_path = strdup("/dev/i2c-1");
    sensor.fd = VL53L0X_i2c_init(i2c_path, sensor.I2cDevAddr);
    if (sensor.fd < 0)
    {
        std::cerr << "ERROR on initializing sensor\n";
        exit(EXIT_FAILURE);
    }

    sensor_status = VL53L0X_DataInit(&sensor);
    check_status(sensor_status);

    sensor_status = VL53L0X_StaticInit(&sensor);
    check_status(sensor_status);

    sensor_status = VL53L0X_PerformRefCalibration(&sensor, &VhvSettings, &PhaseCal);
    check_status(sensor_status);

    sensor_status = VL53L0X_PerformRefSpadManagement(&sensor, &refSpadCount, &isApertureSpads);
    check_status(sensor_status);

    sensor_status = VL53L0X_SetDeviceMode(&sensor, VL53L0X_DEVICEMODE_SINGLE_RANGING);
    check_status(sensor_status);

    sensor_status = VL53L0X_StartMeasurement(&sensor);
    check_status(sensor_status);
}

void export_pwm_chanel()
{
    write_to_file("/sys/class/pwm/pwmchip0/export", "0");
}

void unexport_pwm_channel()
{
    write_to_file("/sys/class/pwm/pwmchip0/unexport", "0");
}

void handle_interrupt(int signal)
{
    std::cout << "\nCaught Ctrl + C\n";
    rotate_servo(0);
    unexport_pwm_channel();
    close_socket();
    exit(EXIT_SUCCESS);
}

void init_interrupt()
{

    sigIntHandler.sa_handler = handle_interrupt;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, nullptr);
}

int main(int argc, char *argv[])
{
    init_interrupt();
    init_socket();
    init_sensor(sensor);

    int delta_angle = atoi(argv[1]);
    assert(delta_angle > 0);
    uint16_t sleep_time = atoi(argv[2]);
    uint16_t distance;

    export_pwm_chanel();
    rotate_servo(0);
    while (1)
    {
        rotate_servo(angle);
        distance = measure_distance(sensor);
        std::string message = std::to_string(angle) + " " + std::to_string(distance) + "\n";
        std::cout << "Send message: " << message;
        send_tcp_message(message);
        angle += delta_angle;
        if (angle <= 0 || angle >= 180)
            delta_angle *= -1;
        usleep(sleep_time);
    }
    return EXIT_SUCCESS;
}
