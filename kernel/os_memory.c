#include "os_memory.h"

/*!
 * Allocates a block of \p size bytes of memory. (KERNEL MODE)
 * \param size Size in bytes of the requested block of memory.
 * \return Memory address to the new memory block
 */
void* os_kernel_malloc(size_t size) {

}

/*!
 * Frees a previously allocated block of memory. (KERNEL MODE)
 * \param ptr Pointer to the memory block to be freed.
 */
void os_kernel_free(void* ptr) {

}

/*!
 * Allocates a block of \p size bytes of memory. (USER MODE)
 * \param size Size in bytes of the requested block of memory.
 * \return Memory address to the new memory block
 */
void* os_user_malloc(size_t size) {

}

/*!
 * Frees a previously allocated block of memory. (USER MODE)
 * \param ptr Pointer to the memory block to be freed.
 */
void os_user_free(void* ptr) {
    
}