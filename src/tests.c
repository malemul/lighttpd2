#include <stdio.h>


#include <lighttpd/base.h>

#include <lighttpd/config_parser.h>

int request_test() {
	liChunkQueue *cq;
	liRequest req;
	liHandlerResult res;

	cq = chunkqueue_new();
	request_init(&req, cq);

	chunkqueue_append_mem(cq, CONST_STR_LEN(
		"GET / HTTP/1.1\r\n"
		"Host: www.example.com\r\n"
		"\r\n"
		"abc"
	));

	res = http_request_parse(NULL, NULL, &req.parser_ctx);
	if (res != LI_HANDLER_GO_ON) {
		fprintf(stderr, "Parser return %i", res);
	}

	assert(req.http_method == LI_HTTP_METHOD_GET);
	assert(cq->length == 3);

	request_clear(&req);
	chunkqueue_free(cq);

	return res == LI_HANDLER_GO_ON ? 0 : 1;
}

int main() {
	liServer *srv;

	guint32 ip, netmask;
	assert(parse_ipv4("10.0.3.8/24", &ip, &netmask, NULL));
	printf("parsed ip: %s\n", inet_ntoa(*(struct in_addr*) &ip));
	printf("parsed netmask: %s\n", inet_ntoa(*(struct in_addr*) &netmask));

	guint8 ipv6[16];
	guint network;
	GString *s;
	assert(parse_ipv6("::ffff:192.168.0.1/80", ipv6, &network, NULL));
	s = ipv6_tostring(ipv6);
	printf("parsed ipv6: %s/%u\n", s->str, network);

	request_test();

	return 0;

	srv = server_new();
	return 0;
}
