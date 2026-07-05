#ifndef KPX_CONFIG_H
#define KPX_CONFIG_H

/*
	@brief Defines if the system supports floats at all 

	@note This is especially important in some micro controllers as they don't have floats 
	at all and using them is extremely expensive
*/
#define KPX_SYSTEM_FLOAT_ENABLED 1

/*
    @brief Defines that the system supports 32bit floats
*/
#define KPX_SYSTEM_32_BIT_FLOAT_SUPPORTED 1

/**
 * @brief defines the maximum size a register has on the platform
 *
 * @note this allows for 8 bit micros to still support certain value groups
*/
#define KPX_SYSTEM_REGISTER_SIZE 64

/**
 * @brief the amount of connectors we allow to be added
 * 
 * @note currently set to 8 which might be way to much for a normal setup
 */
#define KPX_CONNECTOR_COUNT 8

#endif