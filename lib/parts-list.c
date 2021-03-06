#include "parts.h"

typedef struct node {
  Part part;
  struct node *next;
} Node;
struct parts_type {
  size_t count;
  struct node *head;
};
Parts new_db(size_t initial_size)
{
  if (initial_size)
    ;
    /* Taking this arg is a hack to stay polymorphic while enabling
     * variable initial sizes for the array
     * implementations.
     *
     * The empty if just quiets the compiler.
     */
  Parts db = malloc(sizeof(struct parts_type));
  if (db == NULL)
    memory_error(__FILE__, __LINE__, __func__);

  db->count = 0;
  db->head = NULL;

  return db;
}
static void destroy_node(Node *n)
{
  destroy_part(n->part);
  free(n);
}
void destroy_db(Parts db)
{
  Node *next;
  for (Node *n = db->head; n; n = next) {
    next = n->next;
    destroy_node(n);
  }

  free(db);
}
static Node *create_node(Part p)
{
  Node *new_node = malloc(sizeof(Node));
  if (!new_node)
    memory_error(__FILE__, __LINE__, __func__);

  new_node->part = p;
  new_node->next = NULL;
  return new_node;
}
int insert_part(Parts db, Part p)
{
  /* invalid record? */
  if (!p)
    return -3;

  Node *cur, *prev;
  for (cur = db->head, prev = NULL;
       cur != NULL && get_part_number(p) > get_part_number(cur->part);
       prev = cur, cur = cur->next)
    ;
  /* duplicate part number? */
  if (cur != NULL && get_part_number(p) == get_part_number(cur->part))
    return -2;

  Node *n = create_node(p);
  n->next = cur;
  if (prev == NULL)
    db->head = n;
  else
    prev->next = n;

  db->count++;
  return 0;

}
int delete_part(Parts db, PartNumber number)
{
  Node *cur, *prev;

  for (cur = db->head, prev = NULL;
       cur != NULL && number != get_part_number(cur->part);
       prev = cur, cur = cur->next)
    ;
  if (cur == NULL) /* not found */
    return -1;
  if (prev == NULL)  /* first */
    db->head = db->head->next;
  else
    prev->next = cur->next;

  destroy_node(cur);
  db->count--;
  return 0;
}
Part find_part(Parts db, PartNumber part_number)
{
  for (Node *n = db->head; n != NULL; n = n->next)
    if (get_part_number(n->part) == part_number)
      return n->part;

  return NULL;
}
void iterate(Parts db, void (*op)(Part p))
{
  for (Node *n = db->head; n != NULL; n = n->next)
    op(n->part);
}
size_t size(Parts db)
{
  return db->count;
}
Part approximate_part(Parts db, PartNumber part_number)
{
  PartNumber pn;
  Node *previous, *current;

  if (db->count == 0)
    return NULL;

  /* return first if the target is smaller than all */
  if (get_part_number(db->head->part) > part_number)
    return db->head->part;

  /* find whichever the exact part or the next greatest */
  current = db->head;
  while ( current && (pn = get_part_number(current->part)) < part_number) {
    previous = current;
    current  = current->next;
  }
  /* return the exact part or the next smallest */
  if (pn == part_number)
    return current->part;
  else
    return previous->part;
}
