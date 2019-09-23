/* adlist.h - A generic doubly linked list implementation
 *
 * Copyright (c) 2006-2012, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ADLIST_H__
#define __ADLIST_H__

/* Node, List, and Iterator are the only data structures used currently. */
//节点
typedef struct listNode {
    struct listNode *prev;
    struct listNode *next;
    void *value;
} listNode;

//节点迭代器
typedef struct listIter {
    listNode *next;
    int direction;
} listIter;
//链表结构
typedef struct list {
    listNode *head; //头部
    listNode *tail; //尾部
    //实现多态，根据不同的数据类型有不同的dup,free,match
    void *(*dup)(void *ptr); //复制
    void (*free)(void *ptr); //释放
    int (*match)(void *ptr, void *key); //匹配
    unsigned long len; //保存的长度
} list;

/* Functions implemented as macros */
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

//设置-复制、释放、匹配的方法
#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

//获取-复制、释放、匹配的方法
#define listGetDupMethod(l) ((l)->dup)
#define listGetFree(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
list *listCreate(void); //创建list
void listRelease(list *list); //释放list
list *listAddNodeHead(list *list, void *value); //添加到头
list *listAddNodeTail(list *list, void *value); //添加到尾部
list *listInsertNode(list *list, listNode *old_node, void *value, int after); //插入节点
void listDelNode(list *list, listNode *node); //删除节点
listIter *listGetIterator(list *list, int direction); //获取list的迭代器
listNode *listNext(listIter *iter); //下一个节点
void listReleaseIterator(listIter *iter); //释放迭代器
list *listDup(list *orig); //复制list
listNode *listSearchKey(list *list, void *key); //查找key
listNode *listIndex(list *list, long index); //第index个节点
void listRewind(list *list, listIter *li); //将li重置为list头结点，并设置为正向迭代
void listRewindTail(list *list, listIter *li); //将li重置为list的尾节点，并设置为反向迭代
void listRotate(list *list); //将尾结点插入到头结点

/* Directions for iterators */
#define AL_START_HEAD 0 //迭代器的方向
#define AL_START_TAIL 1

#endif /* __ADLIST_H__ */
