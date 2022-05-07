#include "linkedlist.h"


List* createList(){

  struct List* list;

  list = malloc(sizeof(List));
  
  list->NumClient = 0;
  list->first = NULL;
  list->last = NULL;
  return list;

}

void insertList(struct List* list, int data, char* handle){
  
   struct Node* client; 
   client = malloc(sizeof(Node));

   client->data = data;
   strcpy(client->handle, handle);
   client->next = NULL;
   client->prev = NULL;

   if(list->first == NULL){
      list->first = client;
      list->last = client;
   } else {

     list->last->next = client;
     client->prev = list->last;
     list->last = client;

  }
  list->NumClient++;

}

Node* getSocketNumHelper(Node* client, int socketNum){
   if (client->data == socketNum){
        return client;
   }

   if(client->next == NULL){
        return NULL;
   }

   return  getSocketNumHelper(client->next, socketNum); 

}

Node* getClientSocket(struct List* list, int socketNum){
    if(list->first == NULL){
       return NULL;
    } 
    return  getSocketNumHelper(list->first, socketNum); 
 
}


Node* getHandleHelper(Node* client, char* handle){
   if ( !strcmp(client->handle, handle)){
        return client;
   }

   if(client->next == NULL){
        return NULL;
   }
   return  getHandleHelper(client->next, handle); 

}

Node* getClientHandle(struct List* list, char* handle){
    if(list->first == NULL){
       return NULL;
    } 
    return  getHandleHelper(list->first, handle); 
 
}


void removeListHelper(struct List* list, struct Node* current, struct Node* client){
     
       if(current->data == client->data){
         if(current->prev != NULL){
            current->prev->next = current->next;
         }
         if(current->next != NULL){
           current->next->prev = current->prev;
         }
         if(list->first->data == current->data){
            list->first = current->next; 
         }
         if(list->last->data == current->data){
             list->last = current->prev;
       }
         list->NumClient--;
         free(current);
       }else if(current->next != NULL){
    
        removeListHelper(list, current->next, client);

      }
}

void removeList(struct List* list, struct Node* client){
    if(list->first != NULL){
       
       removeListHelper(list , list->first, client);
    }
}


void printListHelper(struct Node * client){


	printf("handle: %s socket number: %d\n", client->handle, client->data);
        if(client->next != NULL){
            printListHelper(client->next);
        }


}

void printList(struct List * list){

    printf("there are %d entrys in the list\n", list->NumClient);

    if(list->first != NULL){
       printListHelper(list->first);

    }

}
