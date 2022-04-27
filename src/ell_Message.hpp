
#include "ell_socketOps.hpp"
#include <cassert>
#include <cstdint>
#include <cstring>

// using message_data = struct {
//     int32_t magic = 0x0717;
//     int32_t type = 1;
//     int32_t length = 0;
//     int32_t checksum = 0;
//     char *content = nullptr;
// };

// class ell_Message {
// public:
//     static message_data create(char *__start, int __length) {
//         message_data message;
//         message.magic = 0x0717;
//         message.type = 1;
//         message.checksum = 0;

//         assert(message.content == nullptr);

//         message.content = new char[__length];
//         ::strncpy(message.content, __start, __length);

//         return message;
//     }

//     static void send_message(message_data message, int fd) {
//         assert(message.content != nullptr);

//         // sockops::send(fd, (void *)message.magic, sizeof(message.magic), 0);
//         // sockops::send(fd, (void *)message.type, sizeof(message.type), 0);
//         // sockops::send(fd, (void *)message.length, sizeof(message.length), 0);
//         // sockops::send(fd, (void *)message.checksum, sizeof(message.checksum, 0),
//         //               0);
//         sockops::send(fd, message.content, message.length, 0);
//     }

//     static void delete_message(message_data &message) {
//         assert(message.content != nullptr);
//         delete message.content;
//         message.content = nullptr;
//     }
// };