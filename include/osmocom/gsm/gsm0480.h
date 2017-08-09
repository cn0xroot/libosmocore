/*! \file gsm0480.h */

#pragma once

#include <osmocom/core/defs.h>
#include <osmocom/core/msgb.h>
#include <osmocom/gsm/protocol/gsm_04_08.h>
#include <osmocom/gsm/protocol/gsm_04_80.h>

/**
 * According to the GSM 04.80 (version 5.0.0) specification Annex A
 * "Expanded ASN.1 Module "SS-Protocol", the maximum size of a USSD
 * OCTET STRING field is 160 bytes.
 */
#define GSM0480_USSD_OCTET_STRING_LEN	160

/**
 * Thus according to ETSI TS 123 038 (version 10.0.0) specification
 * 6.1.2.3 "USSD packing of 7 bit characters", in 160 octets, it's
 * possible to pack (160 * 8) / 7 = 182.8, that is 182 characters.
 * The remaining 6 bits are set to zero.
 */
#define GSM0480_USSD_7BIT_STRING_LEN	182

/**
 * DEPRECATED: this definition doesn't follow any specification,
 * so we only keep it for compatibility reasons. It's strongly
 * recommended to use correct definitions above.
 */
#define MAX_LEN_USSD_STRING		31

/* deprecated */
struct ussd_request {
	char text[MAX_LEN_USSD_STRING + 1];
	uint8_t transaction_id;
	uint8_t invoke_id;
};

/* deprecated */
int gsm0480_decode_ussd_request(const struct gsm48_hdr *hdr, uint16_t len,
				struct ussd_request *request) OSMO_DEPRECATED("Use gsm0480_decode_ss_request() instead");

struct ss_request {
	uint8_t opcode;
	uint8_t ss_code;
	uint8_t ussd_text[GSM0480_USSD_OCTET_STRING_LEN];
	uint8_t transaction_id;
	uint8_t invoke_id;
};

int gsm0480_decode_ss_request(const struct gsm48_hdr *hdr, uint16_t len,
				struct ss_request *request);

struct msgb *gsm0480_create_ussd_resp(uint8_t invoke_id, uint8_t trans_id, const char *text);
struct msgb *gsm0480_create_unstructuredSS_Notify(int alertPattern, const char *text);
struct msgb *gsm0480_create_notifySS(const char *text);
struct msgb *gsm0480_create_ussd_notify(int level, const char *text);
struct msgb *gsm0480_create_ussd_release_complete(void);

int gsm0480_wrap_invoke(struct msgb *msg, int op, int link_id);
int gsm0480_wrap_facility(struct msgb *msg);

struct gsm48_hdr *gsm0480_l3hdr_push(struct msgb *msg, uint8_t proto_discr,
				     uint8_t msg_type);
