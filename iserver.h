#ifndef ISERVER_H
#define ISERVER_H

#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int Server()
{
    int sock, listener;
    struct sockaddr_in addr;
    size_t buf_len = 1024;
    char buf[buf_len];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        std::cerr << "Не удалось создать слушающий сокет!" << std::endl;
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(5050);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "Не удалось связать слушающий сокет с !" << std::endl;
        return 2;
    }

    listen(listener, 1);

    while(1)
    {
        sock = accept(listener, nullptr, nullptr);
        if(sock < 0)
        {
            std::cerr << "Не удалоь создать новый сокет для общения с клинетом!" << std::endl;
            return 3;
        }

        while(1)
        {
            bytes_read = recv(sock, buf, buf_len, 0);
            if(bytes_read <= 0) break;
            send(sock, buf, bytes_read, 0);
        }

        close(sock);
        return 0;
    }
}

#endif // ISERVER_H
