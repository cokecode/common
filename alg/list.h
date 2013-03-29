/*
 * list.h
 * Paul Chen <yongbao.chen@net263.com>
 * 2006-02-07
 *
 */

#ifndef __RDP_LIST_H__
#define __RDP_LIST_H__

#include <stdlib.h>

/*
 * Double-linked list manipulation.
 *
 * list structure:
 *
 *   +---------------------------------------------+
 *   |                                             |
 *   +-> +----+ -> +-----+ -> +-----+ -> +-----+ --+
 *       |head|    |node1|    |node2|    |node3|
 *   +-- +----+ <- +-----+ <- +-----+ <- +-----+ <-+
 *   |                                             |
 *   +---------------------------------------------+
 *
 * 'head' is a placehoder only and can't hold data. data is stored in
 * 'nodeN'.
 */

typedef struct _ListNode
{
    struct _ListNode * prev;
    struct _ListNode * next;
} ListNode;

static __inline void list_reset(ListNode * node)
{
    node->next = node->prev = NULL;
}

static __inline void list_init(ListNode * node)
{
    node->next = node->prev = node;
}

static __inline int list_empty(ListNode * head)
{
    return (head->next == head);
}

static __inline ListNode * list_head(ListNode * head)
{
    if (list_empty(head))
        return NULL;
    return head->next;
}

static __inline ListNode * list_tail(ListNode * head)
{
    if (list_empty(head))
        return NULL;
    return head->prev;
}

/* a simple for-loop */
#define list_for_each(node, head) \
    for (node = (head)->next; node != (head); node = node->next)

/* for-loop that the node can be removed safely.
 * @node    - the iterator
 * @tmp     - temporary variable used to save iterator
 * @head    - you list head
 */
#define list_for_each_safe(node, tmp, head) \
    for (node = (head)->next, tmp = node->next; \
         node != (head); \
         node = tmp, tmp = tmp->next)

#define list_entry(type, member, ptr) \
    container_of(type, member, ptr)

static __inline ListNode * list_node(ListNode * head, int index)
{
    int i = 0;
    ListNode * node;

    list_for_each(node, head) {
        if (i == index)
            return node;

        i++;
    }

    return NULL;
}

static __inline void __list_insert_between(ListNode * prev, ListNode * next,
        ListNode * node)
{
    node->prev = prev;
    node->next = next;
    prev->next = node;
    next->prev = node;
}

static __inline void list_insert(ListNode * head, ListNode * node)
{
    __list_insert_between(head, head->next, node);
}

static __inline void list_append(ListNode * head, ListNode * node)
{
    __list_insert_between(head->prev, head, node);
}

static __inline void list_insert_before(ListNode * pos, ListNode * node)
{
    __list_insert_between(pos->prev, pos, node);
}

static __inline void list_insert_after(ListNode * pos, ListNode * node)
{
    __list_insert_between(pos, pos->next, node);
}

static __inline void __list_remove_between(ListNode * prev, ListNode * next)
{
    prev->next = next;
    next->prev = prev;
}

static __inline void list_remove(ListNode * node)
{
    __list_remove_between(node->prev, node->next);
}

static __inline void list_remove_head(ListNode * head)
{
    if ( ! list_empty(head))
        list_remove(head->next);
}

static __inline void list_remove_tail(ListNode * head)
{
    if ( ! list_empty(head))
        list_remove(head->prev);
}

static __inline void list_clear(ListNode * head)
{
    ListNode * node;

    while (!list_empty(head)) {
        node = list_head(head);
        list_remove_head(head);
        list_reset(node);
    }
}

static __inline size_t list_size(ListNode * head)
{
    ListNode * node;
    int count = 0;

    for (node = head->next; node != head; node = node->next)
        count++;

    return count;
}

#endif /* __RDP_LIST_H__ */
/*
 vim:ts=4:sw=4:ai:cindent:et:tw=80
*/
