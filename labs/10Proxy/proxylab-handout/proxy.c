#include <stdio.h>
#include "csapp.h"
#include "sbuf.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define MAX_THREAD_NUM 10
#define SBUFSIZE 100

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

sbuf_t sbuf;

void *thread(void *vargp);

void process(int fd);

void read_hdrs(rio_t *rp, char *buf, char *host, char *proxy_connection, char *connection, char *user_agent);

void parse_uri(char *uri, char *host, char *port, char *path);

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

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
    connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
    sbuf_insert(&sbuf, connfd);
  }
  return 0;
}

void *thread(void *vargp) {
  Pthread_detach(pthread_self());
  while (1) {
    int fd = sbuf_remove(&sbuf);
    process(fd);
    Close(fd);
  }
}

void process(int fd) {

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

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg) {

  char buf[MAXLINE], body[MAXBUF];
  sprintf(body, "<html><title>Proxy Error</title>");
  sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
  sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
  sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
  sprintf(body, "%s<hr><em>The Proxy Web server</em>\r\n", body);

  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-length: %d\r\n\r\n", (int) strlen(body));
  Rio_writen(fd, buf, strlen(buf));
  Rio_writen(fd, body, strlen(body));
}
