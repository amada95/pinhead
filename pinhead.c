#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define NTP_SERVER_HOSTNAME	"0.ru.pool.ntp.org"	// USER CONFIGURABLE: NTP Server Hostname
#define NTP_SERVER_PORT		123					// USER CONFIGURABLE: NTP Server Port

#define NTP_TIMESTAMP_DELTA	2208988800ull		// Gregorian seconds since UNIX prime epoch (Jan. 1, 1970)

typedef struct {
	uint8_t		li_vn_mode;			// Leap indicator		(2 bits) : Indicator for leap conditions
									// Protocol version		(3 bits) : Protocol version number
									// Protocol mode		(3 bits) : Protocol mode, 3 for clients
									
	uint8_t		loc_stratum;		// Local clock stratum level
	uint8_t		loc_polltime;		// Local clock maximum message interval
	uint8_t		loc_precision;		// Local clock precision
	
	uint32_t	root_delay;			// Source message round trip delay time
	uint32_t	root_dispersion;	// Source message maximum allowed error	
	uint32_t	ref_id;				// Reference clock identifier
	
	uint32_t	ref_s;				// Reference timestamp seconds
	uint32_t	ref_f;				// Reference timestamp fractions of a second
	
	uint32_t	orig_s;				// Originate timestamp seconds
	uint32_t	orig_f;				// Originate timestamp fractions of a second
	
	uint32_t	rx_s;				// Recieved timestamp seconds
	uint32_t	rx_f;				// Recieved timestamp fractions of a second
	
	uint32_t	tx_s;				// Transmit timestamp seconds
	uint32_t	tx_f;				// Transmit timestamp fractions of a second
} ntp_packet;

static void die(const char* msg) {
	perror(msg);
	exit(-1);
}

int main(int argc, char* argv[]) {
	int 				sockfd;
	struct sockaddr_in	server_addr;
	struct hostent*		server;
	
	// Setup packet and socket
	ntp_packet packet = {0};
	memset(&packet, 0, sizeof(ntp_packet));
	*((char*) &packet) = 0x1b;	//set li_vn_mode to 00011011 (li 0 / vn 3 / mode 3)
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		die("error: failed opening socket");
	}
	
	// Configure server address
	memset((char*) &server_addr, 0, sizeof(server_addr));
		server_addr.sin_family = AF_INET;
	if((server = gethostbyname(NTP_SERVER_HOSTNAME)) == NULL) {
			die("error: failed resolving host");
	}
	memcpy((char*) server->h_addr_list[0], (char*) &server_addr.sin_addr.s_addr, server->h_length);
	server_addr.sin_port = htons(NTP_SERVER_PORT);

	// Connect and exchange data with server
	if(connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
		die("error: failed connecting to server");
	}
	if(write(sockfd, (char*) &packet, sizeof(ntp_packet)) < 0) {
		die("error: failed sending data to server");
	}
	if(read(sockfd, (char*) &packet, sizeof(ntp_packet)) < 0) {
		die("error: failed reading data from server");
	}

	// Parse and print time data from packet 
	packet.tx_s = ntohl(packet.tx_s);
	packet.tx_f = ntohl(packet.tx_f);
	time_t tx = (time_t) (packet.tx_s - NTP_TIMESTAMP_DELTA);
	printf("Current Time: %s", ctime((const time_t*) &tx));
	
	return 0;
}
