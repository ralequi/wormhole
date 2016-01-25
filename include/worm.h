#ifndef __WORM_H__
#define __WORM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

	enum DataType {
		CUSTOM = -2, COMPOUND, ARRAY, INT8, UINT8, INT16, UINT16, INT32, UINT32, INT64, UINT64, STRING
	};

	typedef struct {
		enum DataType type;
		union {
			enum DataType arrayType;
			/*TODO:		struct {
						uint32_t size;
						struct ConnectionDataType *elementTypes;
					} compoundType;*/
		} ext ; //extended
	} ConnectionDataType;

	typedef struct {
		uint32_t size;
		uint32_t hash;
		uint16_t category;
		ConnectionDataType *type;
	} MessageInfo;

	typedef struct {
		ConnectionDataType *inputTypes;
		ConnectionDataType *outputTypes;
	} WormConfig;

	/* Name WH_init
	 * Starts the WormHole Library
	 * Return 0 if OK, something else if error.
	 */
	uint8_t WH_init (void);

	/* Name WH_halt
	 * Stops and free the WormHole Library
	 * Return 0 if OK, something else if error.
	 */
	uint8_t WH_halt (void);

	/* Name WH_recv
	 * TODO
	 * Params:
	 * Return the number of bytes readed, 0 if ERROR or none.
	 */
	uint32_t WH_recv (void *data, MessageInfo *mi);

	/* Name WH_send
	 * TODO
	 * Params:
	 * Return 0 if OK, something else if error.
	 */
	uint8_t WH_send (const void *const data, const MessageInfo *const mi);

	/* Name WH_recv_blk
	 * TODO
	 * Params:
	 * Return the number of bytes readed, 0 if ERROR or none.
	 */
	uint32_t WH_recv_blk (void **data, MessageInfo **mi, uint16_t num);

	/* Name WH_send_blk
	 * TODO
	 * Params:
	 * Return 0 if OK, something else if error.
	 */
	uint8_t WH_send_blk (const void **const data, const MessageInfo **const mi, const uint16_t num);


#ifdef __cplusplus
}
#endif
#endif
