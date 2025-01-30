#ifndef __XML_H__
#define __XML_H__
#define BUFFER_SIZE 1000
#include "../texture.h"

template <class T> class ListNode{
   ListNode<T> *before, *after;
   ListNode<T>(T t){
      data = t;
   }
   T data;
};

template <class T> class LinkedList{
   unsigned int size;
   ListNode<T> *start, *end;
   T getFirst(){
      return peekFirst(); 
   }
   T getLast(){
      return peekLast(); 
   }
   void push(T t){
      addLast(t);
   }
   T pop(){
      return pollLast();
   }
   T peek(){
      return peekFirst(); 
   }
   T peekFirst(){
      return start->data; 
   }
   T peekLast(){
      return end->data; 
   }
   T poll(){
      return pollFirst();
   }
   T pollFirst(){
      ListNode<T> *temp = start;
      size--;
      start = temp->after;
      T t = temp->data;
      temp->~ListNode<T>();
      return t;
   }
   T pollLast(){
      ListNode<T> *temp = end;
      size--;
      end = temp->before;
      T t = temp->data;
      temp->~ListNode<T>();
      return t;
   }
   void add(T t){
      addLast(t);
   }
   void addFirst(T t){
      if(size==0){
         start = end = new ListNode<T>(t);
      }
      else{
         ListNode<T> *t = new ListNode<T>(t);
         t->next = start;
         start = t;
      }
      size++;
   }
   void addLast(T t){
      if(size==0){
         start = end = new ListNode<T>(t);
      }
      else{
         end->next = new ListNode<T>(t);
         end = end->next;
      }
      size++;
   }
   T get(int i){
      if(i>=size/2){
         int pos = size-1;
         ListNode<T> *cur = end;
         while(pos>i){ cur = cur->before; pos--;}
         return cur;
      }
      else{
         int pos = 0;
         ListNode<T> *cur = start;
         while(pos<i){ cur = cur->next; pos++;}
         return cur;
      }
      return 0;
   }
   LinkedList(){size = 0;}
   ~LinkedList(){
      while(size>0){
         ListNode<T> *ln = start;
         start = start->next;
         ln->~ListNode<T>();
         size--;
      }
   }
};

template <class A,class B> class ListMapNode{
   ListMapNode<A,B> *before, *after;
   A first;
   B second;
   ListMapNode<A,B>(A a,B b){
      first = a;
      second = b;
   }
};

class XMLParser{
  
  
};

class XMLNode{
   LinkedList<XMLNode*> children;
   XMLNode* parent;
   XMLNode(XMLNode* a):children(){
      parent = a;
   }
};

LinkedList<Shape*>* parseFile(FILE* f){
   LinkedList<Shape*>* data = new LinkedList<Shape*>();
   LinkedList<void*>* stack = new LinkedList<void*>();
   char next = ' ';
   char buffer[BUFFER_SIZE];
   int pos;
   while(next!=EOF){
      do{
         next = getc(f);
      }while(next==' ' || next=='\n' || next=='\t' || next=='\r' || next==EOF);
      pos = 0;
      while(true){
         next = getc(f);
         if(next==' ' || next=='\n' || next=='\t' || next=='\r' || next==EOF) break;
         if(pos==200){
            fprintf(err, "Name overflow error at %s\n", buffer);
            exit(1);
         }
         buffer[BUFFER_SIZE] = next;
         pos++;
      }
      if(pos==0) break;
      switch(buffer[0]){
      
      
      }
   }
}
#endif