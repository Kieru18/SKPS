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

int server_socket;
int client_socket;
int port = 8000;
int angle = 0;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
struct sigaction sigIntHandler;

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
    std::cout << "Waiting for TCP connection on port " << port << "\n";

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
        exit(-1);
    }
}

void close_socket()
{
    std::cout << "Closing socket"
              << "\n";
    close(client_socket);
    close(server_socket);
}

uint16_t measure_distance()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<uint16_t> dist(0, 90);
    uint16_t delta_dist = dist(mt);
    uint16_t distance = std::pow(std::cos((angle / 90.f) * M_PI), 2) * 200 + delta_dist;
    if (distance < 0) distance = 0;
    std::cout << "Measured FAKE distance: " << distance << "\n";
    return distance;
}

void rotate_servo(uint16_t destination)
{
    std::cout << "Rotate FAKE servo to angle: " << destination << "\n";
    usleep(100000);
}

void handle_interrupt(int s)
{
    std::cout << "\nCaught Ctrl + C\n";
    rotate_servo(0);
    close_socket();
    exit(0);
}

int main(int argc, char *argv[])
{
    sigIntHandler.sa_handler = handle_interrupt;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, nullptr);

    init_socket();

    int delta_angle = atoi(argv[1]);
    assert(delta_angle > 0);
    uint16_t sleep_time = atoi(argv[2]);
    uint16_t distance;

    rotate_servo(0);
    while (1)
    {
        rotate_servo(angle);
        distance = measure_distance();
        std::string message = std::to_string(angle) + " " + std::to_string(distance) + "\n";
        std::cout << "Send message: " << message;
        send_tcp_message(message);
        angle += delta_angle;
        if (angle <= 0){
            angle = 0;
            delta_angle *= -1;}
        else if(angle >= 180){
            angle = 180;
            delta_angle *= -1;}
        usleep(sleep_time);
    }
    return 0;
}
