#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    list_ele_t *temp;

    if (!q)
        return;

    while (q->head != NULL) {
        temp = q->head->next;
        free(q->head->value);
        free(q->head);
        q->head = temp;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;

    if (!q)
        return false;

    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    uint32_t string_size = strlen(s);
    newh->value = (char *) malloc(string_size + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    memcpy(newh->value, s, string_size);
    newh->value[string_size] = '\0';

    newh->next = q->head;
    q->head = newh;
    if (q->size == 0)
        q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;

    if (!q)
        return false;

    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;

    uint32_t string_size = strlen(s);
    newh->value = (char *) malloc(string_size + 1);
    if (newh->value == NULL) {
        free(newh);
        return false;
    }
    memcpy(newh->value, s, strlen(s));
    newh->value[string_size] = '\0';
    newh->next = NULL;

    if (q->tail != NULL)
        q->tail->next = newh;
    q->tail = newh;
    if (q->size == 0)
        q->head = newh;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *temp;

    if (!q || !q->size)
        return false;

    if (sp) {
        uint32_t size_to_copy = strlen(q->head->value);
        if (size_to_copy > bufsize - 1)
            size_to_copy = bufsize - 1;
        memcpy(sp, q->head->value, size_to_copy);
        sp[size_to_copy] = '\0';
    }

    temp = q->head;
    q->head = q->head->next;
    q->size--;
    if (q->size == 0)
        q->tail = NULL;
    free(temp->value);
    free(temp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q)
        return q->size;
    return 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    list_ele_t *current, *new, *temp;

    if (!q || !q->size)
        return;

    current = q->head;
    new = NULL;
    for (int i = 0; i < q->size; i++) {
        temp = current->next;
        current->next = new;
        new = current;
        current = temp;
    }
    q->tail = q->head;
    q->head = new;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
static list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t head;
    list_ele_t *curr = &head;

    while (l1 && l2) {
        if (strcasecmp(l1->value, l2->value) >= 0) {
            curr->next = l2;
            curr = curr->next;
            l2 = l2->next;
        } else {
            curr->next = l1;
            curr = curr->next;
            l1 = l1->next;
        }
    }

    if (l1)
        curr->next = l1;
    if (l2)
        curr->next = l2;

    return head.next;
}

static list_ele_t *mergeSort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l1 = mergeSort(head);
    list_ele_t *l2 = mergeSort(fast);

    return merge(l1, l2);
}

void q_sort(queue_t *q)
{
    if (!q || !q->size)
        return;

    q->head = mergeSort(q->head);

    q->tail = q->head;
    while (q->tail->next)
        q->tail = q->tail->next;
}
