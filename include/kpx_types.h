#ifndef KPX_TYPES_H
#define KPX_TYPES_H

#include "../config/kpx_config.h"

#if KPX_SYSTEM_FLOAT_ENABLED == 1
	#if KPX_SYSTEM_64_BIT_FLOAT_SUPPORTED
		typedef double kpx_decimal_t; 
	#elif KPX_SYSTEM_32_BIT_FLOAT_SUPPORTED
		typedef float kpx_decimal_t;
    #else
        #error No Float type defined cant compile with float enabled
	#endif
#endif

#ifdef KPX_SYSTEM_REGISTER_SIZE
	#if KPX_SYSTEM_REGISTER_SIZE == 64
		typedef long long int kpx_signed_number_t;
		typedef unsigned long long int kpx_unsigned_number_t;
	#else
		typedef int kpx_signed_number_t;
		typedef unsigned int kpx_unsigned_number_t;
	#endif
#else
	#error No system register size defined invalid configuration
#endif

typedef unsigned char kpx_byte_t;
typedef kpx_byte_t kpx_bool_t;

typedef struct kpx_fundamental_t {
	enum kpx_fundamental_type_e {
		KPX_FUNDAMENTAL_UNSIGNED_NUM,
		KPX_FUNDAMENTAL_SIGNED_NUM,
#if KPX_SYSTEM_FLOAT_ENABLED
		KPX_FUNDAMENTAL_DECIMAL_NUM,
#endif
		KPX_FUNDAMENTAL_ARRAY
	} type;

	union {
		kpx_unsigned_number_t unum;
		kpx_signed_number_t inum;
#if KPX_SYSTEM_FLOAT_ENABLED
		kpx_decimal_t fnum;
#endif
		kpx_byte_t array[KPX_SYSTEM_REGISTER_SIZE];
	} any;
} kpx_fundamental_t;

typedef struct kpx_builder_t {
	/* tbd */
} kpx_builder_t;

typedef struct kpx_value_t {
    enum kpx_value_type_e {
        KPX_BUILDER,
        KPX_FUNDAMENTAL
    } type;
    union {
        kpx_builder_t builder;
       kpx_fundamental_t fundamental;
    } any;
} kpx_value_t;

/*###################################################################################################################*/

typedef enum kpx_error_e {
    KPX_RET_OK,
    KPX_RET_ERROR
} kpx_error_e;

/*###################################################################################################################*/

/*
	Key Info:
		The key contains all domain information:
		- The key contains an id for data identification
		- The key holds the process local memory addresses 
		- The key contains data to say to which process it belongs
		Keys can contain wild cards but only for subscribers 
		Keys are not allowed to be not unique for publishers, requesters and responders 
*/
typedef struct kpx_key_t kpx_key_t;

typedef struct kpx_sub_t kpx_sub_t;

typedef struct kpx_pub_t kpx_pub_t;

typedef struct kpx_stream_t kpx_stream_t;

typedef struct kpx_connector_t kpx_connector_t;

/*###################################################################################################################*/

#define KPX_POLLING
#define KPX_INTERNAL

/*###################################################################################################################*/

typedef kpx_error_e (*kpx_rpc_cb)();
typedef kpx_error_e (*kpx_fwcb)(kpx_value_t* value);
typedef kpx_error_e (*kpx_cb)(kpx_key_t key, kpx_value_t* value);

typedef kpx_error_e (*kpx_rx_fnc)(kpx_byte_t* bytes, kpx_unsigned_number_t byte_count);
typedef kpx_error_e (*kpx_tx_fnc)(kpx_byte_t* bytes, kpx_unsigned_number_t byte_count);
typedef kpx_unsigned_number_t (*kpx_mtu_fnc)(void);

typedef kpx_unsigned_number_t (*kpx_get_process_id_fnc);

typedef kpx_error_e (*kpx_stream_cb)(kpx_byte_t* bytes, kpx_unsigned_number_t byte_count);

/*###################################################################################################################*/

typedef kpx_unsigned_number_t kpx_domain_t;
typedef kpx_unsigned_number_t kpx_process_id_t;
typedef kpx_unsigned_number_t kpx_group_id_t;
typedef kpx_unsigned_number_t kpx_ms_t;
typedef kpx_unsigned_number_t kpx_mtu_size_t;
typedef kpx_unsigned_number_t kpx_connector_handle_t;

/*###################################################################################################################*/

struct kpx_key_t {
	kpx_domain_t domain;
	kpx_process_id_t process;
	kpx_group_id_t group;
	kpx_unsigned_number_t id;

	KPX_INTERNAL kpx_ms_t last_update;
	KPX_INTERNAL kpx_key_t* next;
};

typedef struct kpx_fw_t {
	KPX_INTERNAL kpx_key_t source;
	KPX_INTERNAL kpx_key_t destination;
} kpx_fw_t;

struct kpx_sub_t {
	KPX_INTERNAL 
		kpx_key_t key;
		kpx_cb cb;
		kpx_sub_t* next;
};

struct kpx_pub_t {
	KPX_INTERNAL 
		kpx_key_t key;

		kpx_sub_t* subscribers;
		kpx_unsigned_number_t subscribers_count;
};

typedef struct kpx_permission_t {
	kpx_unsigned_number_t id;
} kpx_permission_t;

struct kpx_connector_t {
	kpx_domain_t domain;
	kpx_rx_fnc rx;
	kpx_tx_fnc tx;
	kpx_mtu_fnc size;

	kpx_byte_t* base;
};

typedef struct kpx_router_t {
	kpx_pub_t* local_publishers;
	kpx_unsigned_number_t publisher_count;

	kpx_key_t* keys;
	kpx_unsigned_number_t key_count;

	kpx_connector_t connectors[KPX_CONNECTOR_COUNT];
	kpx_unsigned_number_t connector_count;

} kpx_router_t;

#endif