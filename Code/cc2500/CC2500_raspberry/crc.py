poly = 0x83
table = []

def CRC8_computeTable():
    for i in range(256):
        temp = i
        for j in range(8):
            if (temp & 0x80) != 0:
                temp = (temp << 1) ^ poly
            else:
                temp <<= 1

        table.append(temp)


def CRC8_computeChecksum(bytes: list):
    crc = 0

    for i in range(len(bytes)):
        temp = crc ^ bytes[i]
        crc = table[temp]

    return crc

CRC8_computeTable()

print(CRC8_computeChecksum([104]))
print(CRC8_computeChecksum([104]))
