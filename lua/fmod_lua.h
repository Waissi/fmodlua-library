#include "lua.h"
#include "lauxlib.h"
#include "fmod.h"
#include "fmod_studio.h"
#include <stdlib.h>

lua_State *luaState = NULL;
FMOD_STUDIO_SYSTEM *studioSystem = NULL;

struct eventCallback
{
    FMOD_STUDIO_EVENTINSTANCE *event;
    int registryIndex;
    struct eventCallback *next;
};
struct eventCallback *headCallback = NULL;

static void ReleaseEventCallbackList()
{
    while (headCallback)
    {
        struct eventCallback *current = headCallback;
        headCallback = current->next;
        free(current);
    }
}

static struct eventCallback *RetrieveEventCallback(FMOD_STUDIO_EVENTINSTANCE *event)
{
    struct eventCallback *current = headCallback;
    if (headCallback == NULL)
    {
        return NULL;
    }
    while (current->event != event)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current;
}

static void ReleaseEventCallback(struct eventCallback *eventCallback)
{
    struct eventCallback *current = headCallback;
    if (headCallback == eventCallback)
    {
        headCallback = NULL;
        free(eventCallback);
        return;
    }
    while (current->next != eventCallback)
    {
        if (current->next == NULL)
        {
            return;
        }
        else
        {
            current = current->next;
        }
    }
    current->next = current->next->next;
    free(current->next);
}