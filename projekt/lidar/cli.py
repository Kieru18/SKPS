import socket
import math
from matplotlib import pyplot as plt


HOST = "10.42.0.124"
PORT = 8888
MAX_ANGLE = 180
MAX_DIST = 500


def calc_coords(angle, distance):
    x = distance * math.sin(math.radians(angle))
    y = distance * math.cos(math.radians(angle))

    return x, y


def init_plot():
    plt.ion()
    fig, ax = plt.subplots(figsize=(10, 10))

    plt.title("lidar")
    # plt.gca().set_aspect('equal')

    ax.set_xlim(-10, MAX_DIST)
    ax.set_ylim(-MAX_DIST, MAX_DIST)
    plt.show()

    return fig, ax


def update_plot(figure, lines, angle, distance):
    x, y = calc_coords(angle, distance)

    lines[angle].set_data([0, x], [0, y])
    lines[angle].set_color('blue')

    figure.canvas.draw_idle()
    figure.canvas.flush_events()


def main():
    fig, ax = init_plot()
    lines = [ax.plot([0, 0], [0, 0], '-o')[0] for _ in range(MAX_ANGLE + 1)]

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))

        while True:
            data = s.recv(256).decode("utf-8").split(" ")
            try:
                angle = int(data[0])
                distance = int(data[1])
                update_plot(fig, lines, angle, distance)
                print(angle, distance)
            except:
                print("Invalid data input")
                s.close

if __name__ == "__main__":
    main()
