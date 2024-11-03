import numpy as np


def checkCRC(byte_arr):
    byte_arr[0] -= 1
    lenght = byte_arr[0]

    crc1 = np.uint8(0)
    for i in range(lenght):
        crc1 = np.uint8(crc1 + byte_arr[i])

    crc2 = np.uint8(0)
    for i in range(lenght):
        pass
        # crc2 = np.uint8(crc2 ^ byte_arr[i])

    crc = np.uint8(crc1 ^ crc2)

    return crc == byte_arr[lenght]


if __name__ == "__main__":
    test_arr = [6, 18, 1, 20, 2, 46]

    print(checkCRC(test_arr))