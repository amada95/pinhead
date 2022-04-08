#include <stdint.h>
#include <string.h>

typedef struct {
	uint32_t	s;		// Timestamp seconds
	uint32_t	f;		// Timestamp fractions of a second
} ntp_ts;

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
	
	ntp_ts		ref;				// Reference timestamp
	ntp_ts		origin;				// Originate timestamp
	ntp_ts		rx;					// Recieved timestamp
	ntp_ts		tx;					// Transmit timestamp
} ntp_packet;

