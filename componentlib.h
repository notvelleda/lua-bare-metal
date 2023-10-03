#pragma once

struct component {
    // the name of this component
    const char *name;
    // the UUID of this component
    const char *address;

    struct method *first_method;
    struct method *last_method;

    struct component *next;
};

struct method {
    // the name of this method
    const char *name;
    // invokes this method on its component. arguments are passed in a table in index 3 of the stack
    int (*invoke)(lua_State *L);
    uint8_t flags;

    struct method *next;
};

#define METHOD_DIRECT 1
#define METHOD_GETTER 2
#define METHOD_SETTER 4

int luaopen_component(lua_State *L);
struct component *new_component(const char *name, const char *address);
void add_method(struct component *component, const char *name, int (*invoke)(lua_State *L), uint8_t flags);
