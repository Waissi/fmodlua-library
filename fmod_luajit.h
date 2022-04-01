#include <stdlib.h>
#include <stdbool.h>
#include "fmod_studio.h"

FMOD_STUDIO_SYSTEM* studioSystem;

struct bankNode
{
   FMOD_STUDIO_BANK* bank;
   int key;
   struct bankNode *next;
};
struct bankNode* headBank = NULL;
int numBanks = 0;

struct eventNode
{
   FMOD_STUDIO_EVENTINSTANCE* event;
   int key;
   struct eventNode* next;
};
struct eventNode* headEvent = NULL;
int numEvents = 0;

struct busNode
{
   FMOD_STUDIO_BUS* bus;
   int key;
   struct busNode* next;
};
struct busNode* headBus = NULL;
int numBus = 0;

struct vcaNode
{
   FMOD_STUDIO_VCA* vca;
   int key;
   struct vcaNode* next;
};
struct vcaNode* headVCA = NULL;
int numVCA = 0;


FMOD_STUDIO_EVENTINSTANCE* RetrieveEvent(int key)
{
    struct eventNode* current = headEvent;
    if(headEvent == NULL){return NULL;}
    while(current->key != key)
    {
        if(current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current->event;
}

FMOD_STUDIO_BUS* RetrieveBus(int key)
{
    struct busNode* current = headBus;
    if(headBus == NULL){return NULL;}
    while(current->key != key)
    {
        if(current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current->bus;
}

FMOD_STUDIO_VCA* RetrieveVca(int key)
{
    struct vcaNode* current = headVCA;
    if(headVCA == NULL){return NULL;}
    while(current->key != key)
    {
        if(current->next == NULL)
        {
            return NULL;
        }
        else
        {
            current = current->next;
        }
    }
    return current->vca;
}