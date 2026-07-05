/*
	The module shall have a prefix called kpx (Key Performance Xchange) 
	KPX is a comprehensive and powerful solution, capable of integrating and managing complex systems efficiently.
	handling connections even over multiple devices.
*/

/*
	System Level:
		Shall provide a interconnect that can be used across devices with a unspecified bus
		Shall have a low latency (<5 ms) for on device in memory transactions 
		Shall have a rights management system with a white list and black list as well as level (domain) principle

	Goals:
		KPX tries to allow for:
			- minimal footprint
				- meaning support of register sizes that are under 32 bit
			- extensible transport layer
			- modular domain access
			- single subscriber / connection multiple key subscriptions
			- single publisher / connection multiple key publishing
			- multi publisher support 
			- remote procedure support
			- kernel like processing and republishing with different key of data source keys
			- provide ways to define custom allocators for every place where allocation is needed
			- be usable in automotive applications
			- support for variable length data streams (stream)
			- provide extensive rights management even for sub/pub/rpc/stream
		KPX does not try to:
			- providing sophisticated data interfaces
			- providing best in class performance for memory to memory IPC
			- providing unlimited chaining of nodes
			- being completely centralized
			- use any prior created interchange format
			- Contain millions of different connectors
			- Provide meta information like:
				- size information of values from keys
				- unit information of values from keys
				- description of keys
			- Compress in any form data over any bus no matter what mtu size is defined
			  - can be done by connectors if needed
			  - or extended in a more "bloated version"
			- does not version interfaces it expects all to be the same 
				- this is currently the case but might change later
			
		KPX currently:
			- Does not contain connector implementations except memory one
			- Does currently not implement a multi device transport 

	
*/
#include "kpx_types.h"

kpx_error_e kpx_create_forwarder(kpx_fw_t* pRetFw, kpx_fwcb* cb);
kpx_error_e kpx_remove_forwarder(kpx_fw_t fw);


kpx_error_e kpx_subsribe_to(kpx_sub_t* pRetSub, kpx_cb* cb, kpx_bool_t is_async);
kpx_error_e kpx_remove_sub(kpx_sub_t sub);
kpx_error_e kpx_subscriber_has_data_available(kpx_sub_t sub, kpx_key_t* pKey);
kpx_error_e kpx_subscriber_retrive(kpx_sub_t sub, kpx_key_t* pKey, kpx_fundamental_t val);
kpx_error_e kpx_subscriber_retrive_pod(kpx_sub_t sub, kpx_key_t* pKey, kpx_builder_t val);
//kpx_error_e kpx_subsribe_to_domain(kpx_sub_t sub, kpx_domain_t domain, kpx_cb* cb, kpx_bool_t is_async);
//kpx_error_e kpx_subsribe_to_group(kpx_sub_t sub, kpx_group_id_t group, kpx_cb* cb, kpx_bool_t is_async);

kpx_error_e kpx_remove_pub(kpx_pub_t pub);
kpx_error_e kpx_publish_to(kpx_pub_t* pRetPub, kpx_key_t* pKey, kpx_permission_t perm);
kpx_error_e kpx_publish(kpx_pub_t pub, kpx_fundamental_t val);
kpx_error_e kpx_publish_pod(kpx_pub_t pub, kpx_key_t pKey, kpx_builder_t val);

kpx_error_e kpx_register_responder(kpx_key_t key, kpx_rpc_cb* kpx_rpc_cb, kpx_bool_t is_async);
KPX_POLLING kpx_error_e kpx_check_has_request(kpx_key_t key);
KPX_POLLING kpx_error_e kpx_respond(kpx_key_t key, kpx_fundamental_t val);
KPX_POLLING kpx_error_e kpx_respond_pod(kpx_key_t key, kpx_builder_t val);
kpx_error_e kpx_unregister_responder(kpx_key_t key);

kpx_error_e kpx_request(kpx_key_t key, kpx_fundamental_t** val);
kpx_error_e kpx_request_pod(kpx_key_t key, kpx_builder_t** val);

kpx_error_e kpx_create_stream(kpx_stream_t* pRetStream, kpx_key_t Key);
kpx_error_e kpx_remove_stream(kpx_stream_t Stream);
kpx_error_e kpx_stream_to(kpx_stream_t Stream, kpx_unsigned_number_t Byte_Count, kpx_byte_t* pBytes);

kpx_error_e kpx_watch_stream(kpx_key_t Key, kpx_stream_cb cb);
kpx_bool_t kpx_stream_is_empty(kpx_key_t Key);

/*
	Router Info:
		The router shall not be invoked when requests and subscriptions are local or memory based
		The router does handle remote dispatches of registered functions
		The router shall only be used to transfer data to other devices
		The router shall handle any publishing of information across device domains
		The router shall forward messages from connectors
		The router shall have a similar transmission behavior as a hub on startup except if key is explicitly local
		The router shall discover other device keys and discard non subscribed information till subscribed
		The router shall handle device to device communication with appropriate protocols for each mtu
	
	Router Description:
		It's the permission checker and authority for keys and forwarder for inter domain transactions
		It is also capable of dispatching on change to forwarders
		
		This means the router does:
			For publishers:
				Keep a references to all there keys
				Keep references to which domain a key belongs
				Keep a references to rights of keys
				Prevent duplication of keys
				Prevent not allowed access
				Handles non local transactions through connectors
				Handles forward processing units 
			For subscribers:
				checks existence of keys
				checks permissions to access
				forwards connector transactions to it as quasi publisher
			For requester / requests:
				routes requests to repsonder's
				checks permissions to call'e 
			For responder:
				routes requests to them
				stores a list of responder's
				creates call context
				checks permissions of caller
			For itself:	
				exchange key information to other routers in different domains
				exchange permission information to other routers in diffrent domains
				exchange published information for other domains
*/

/*
	the function is required to reject instancing of multiple routers on one device
	the function shall return the single device instance if already created
	the function has to always return a router instance
*/
kpx_error_e kpx_create_router(kpx_router_t* pRet_Router, kpx_get_process_id_fnc proc_);

/*
	the function shall return all available domains on the router
	the function shall only return registered domains/connectors
*/
kpx_error_e kpx_router_domains(kpx_domain_t* pRet_Domains, kpx_signed_number_t* pRet_domain_count);

/*
	the function shall stop any ongoing transactions to other devices
	the function shall clear all state and make it possible to recreate the router instance
	the function shall take a per value instance of #kpx_router_t 
*/
kpx_error_e kpx_kill_router(kpx_router_t* router);

kpx_error_e kpx_appped_memory_connector(void);
kpx_error_e kpx_append_process_connector(void);
kpx_error_e kpx_append_connector(kpx_router_t router, kpx_connector_t connector, kpx_connector_handle_t* pRet_Handle, kpx_mtu_size_t size);
kpx_error_e kpx_remove_connector(kpx_router_t router, kpx_connector_handle_t handle);