#include <stdio.h>

#define get_stack_direction() get_stack_direction_internal(NULL)

/**
 * Negative value: stack grows down (to lower address)
 */
int get_stack_direction_internal(char* addr)
{
    printf("%p\n", addr);
    char dummy;
    if (addr == 0) {
        addr = &dummy;
        get_stack_direction_internal(addr);
    } else {
        return &dummy - addr;
    }
}

int main()
{
    printf("%d\n\n", get_stack_direction());

    printf("%d\n", get_stack_direction());
}
