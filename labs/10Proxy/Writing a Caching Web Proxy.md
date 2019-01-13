# Writing a Caching Web Proxy

- 这个lab主要是要实现一个网络代理服务器，事实上这类代理服务器对于client来说是一个server，对于真实的server来说是一个client，因此同时扮演着两个角色。

- 这个练习的设计覆盖面比较广，不仅是网络编程的client/server，还有并发编程的多线程，以及缓存设计。虽然涉及面比较广，但是总体而言难度不大，多数实现都有样例参考。

- handout中的tiny server是整个实现的基础，csapp.h中包含了很多可以使用的包装函数。

## Part I: Implementing a sequential web proxy

### 说明
- 首先是实现一个顺序处理的代理服务器，也就是说只能同时保持一个连接。
- 大体逻辑是接受client请求，解析后请求server，得到response后再转发回client。
- 框架代码和tiny server保持一致，解析hdrs和uri是接受client请求的主要处理逻辑，格式化成请求server的request。
- 关于response的转发需要小心处理。

### read_hdrs
```c
void read_hdrs(rio_t *rp, char *hdr, char *host, char *proxy_connection, char *connection, char *user_agent) {
  char buf[MAXBUF];

  Rio_readlineb(rp, buf, MAXLINE);
  printf("%s", buf);
  while (strcmp(buf, "\r\n")) {
    if (!memcmp(buf, "Host: ", 6)) {
      char *temp = buf + 6;
      if (strstr(temp, ":") == NULL) {
        strncpy(host, temp, strlen(temp) - 2);
      } else {
        strncpy(host, temp, strstr(temp, ":") - temp);
      }
    } else if (!memcmp(buf, "Proxy-Connection: ", 18)) {
      strncpy(proxy_connection, buf, strlen(buf));
    } else if (!memcmp(buf, "Connection: ", 12)) {
      strncpy(connection, buf, strlen(buf));
    } else if (!memcmp(buf, "User-Agent: ", 12)) {
      strncpy(user_agent, buf, strlen(buf));
    } else {
      strcat(hdr, buf);
    }
    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
  }
  return;
}
```

### parse_uri
```c
void parse_uri(char *uri, char *host, char *port, char *path) {
  int index;

  if (!memcmp(uri, "http://", 7)) {
    uri += 7;
  }
  if (strstr(uri, ":") == NULL) {
    index = strstr(uri, "/") - uri;
    if (strlen(host) == 0) {
      strncpy(host, uri, index);
    }
    strncpy(port, "80", 2);
  } else {
    index = strstr(uri, ":") - uri;
    if (strlen(host) == 0) {
      strncpy(host, uri, index);
    }
    uri = strstr(uri, ":") + 1;
    index = strstr(uri, "/") - uri;
    strncpy(port, uri, index);
  }
  strncpy(path, uri + index, MAXLINE);
}
```

### doit
```c
void doit(int fd) {
  char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE], hdrs[MAXLINE];
  rio_t rio, rio_out;
  char host[MAXBUF], port[MAXBUF], path[MAXBUF], proxy_connection[MAXBUF], connection[MAXBUF], user_agent[MAXBUF];
  int clientfd;
  char request[MAXLINE];
  char response[MAXLINE];

  Rio_readinitb(&rio, fd);
  if (!Rio_readlineb(&rio, buf, MAXLINE)) return;
  printf("%s", buf);
  sscanf(buf, "%s %s %s", method, uri, version);
  if (strcasecmp(method, "GET")) {
    clienterror(fd, method, "501", "Not Implemented", "Proxy does not implement this method");
    return;
  }

  read_hdrs(&rio, hdrs, host, proxy_connection, connection, user_agent);
  parse_uri(uri, host, port, path);

  fprintf(stderr, "host: %s, port: %s, path: %s\n", host, port, path);
  clientfd = Open_clientfd(host, port);
  Rio_readinitb(&rio_out, clientfd);
  sprintf(request, "GET %s HTTP/1.0\r\n", path);
  sprintf(request, "%sHost: %s\r\n", request, host);

  if (strlen(user_agent) == 0) sprintf(request, "%s%s", request, user_agent_hdr);
  else sprintf(request, "%s%s", request, user_agent);

  if (strlen(connection) == 0) sprintf(request, "%sConnection: close\r\n", request);
  else sprintf(request, "%s%s", request, connection);

  if (strlen(proxy_connection) == 0) sprintf(request, "%sProxy-Connection: close\r\n", request);
  else sprintf(request, "%s%s", request, proxy_connection);

  sprintf(request, "%s%s", request, hdrs);
  sprintf(request, "%s\r\n", request);
  fprintf(stderr, "request: %s", request);
  Rio_writen(clientfd, request, strlen(request));
  int count = MAXBUF;
  while (count == MAXBUF) {
    count = Rio_readnb(&rio_out, response, MAXBUF);
    //Fputs(request, stdout);
    Rio_writen(fd, response, count);
  }
  Close(clientfd);
}
```

## Part II: Dealing with multiple concurrent requests

### 说明
- 关于实现并发服务器有很多解决方案，多进程并发、IO多路复用、预线程并发等。其中多进程并发由于进程控制、IPC开销等问题很少被使用，其余两种都是事件驱动并发编程方式，这里采用预线程并发。

- 预线程并发是指服务器启动时创建一定数量的工作线程，他们都阻塞在等待一个线程安全的缓冲区上，主线程将创建的连接插入到这个缓冲区中，由工作线程消费。

### 实现

```c
int main(int argc, char **argv) {
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t tid;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);
  sbuf_init(&sbuf, SBUFSIZE);

  for (int i = 0; i < MAX_THREAD_NUM; i++)
    Pthread_create(&tid, NULL, thread, NULL);
  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA * ) & clientaddr, &clientlen);
    sbuf_insert(&sbuf, connfd);
  }
  return 0;
}
```

```c
void *thread(void *vargp) {
  Pthread_detach(pthread_self());
  while (1) {
    int fd = sbuf_remove(&sbuf);
    process(fd);
    Close(fd);
  }
}
```

## Part III: Caching web objects

