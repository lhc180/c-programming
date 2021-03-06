#include <stdlib.h>
#include "error.h"
#include "part-type.h"


Part new_part(void)
{
  Part p = malloc(sizeof(struct part_type));
  if (!p)
    memory_error(__FILE__, __LINE__, __func__);

  return p;
}
Part set_part(PartNumber number, char * name, PartQuantity on_hand, PartPrice price)
{
  Part p = new_part();

  if (!( set_part_number(p, number) &&
         set_part_on_hand(p, on_hand) &&
         set_part_price(p, price) ))
  {
    destroy_part(p);
    return NULL;
  }

  set_part_name(p, name);

  return p;
}
static bool is_in_range(int field_value)
{
  if (field_value < 0 || field_value > INT_MAX)
    return false;

  return true;
}
bool set_part_number(Part p, PartNumber number)
{
  if (is_in_range(number)) {
    p->number = number;
    return true;
  }
  return false;
}
void set_part_name(Part p, char *name)
{
  strcpy(p->name,  name);
  p->name[NAME_LEN] = '\0';  /* terminate string field, no matter what */
}
bool set_part_on_hand(Part p, PartQuantity on_hand)
{
  if (is_in_range(on_hand)) {
    p->on_hand = on_hand;
    return true;
  }
  return false;
}
bool change_part_on_hand(Part p, PartQuantity change)
{
  if (set_part_on_hand(p, p->on_hand  + change))
    return true;

  return false;
}
bool set_part_price(Part p, PartPrice price)
{
  if (is_in_range(price)) {
    p->price = price;
    return true;
  }
  return false;
}
PartNumber get_part_number(Part p)  {return p->number; }
char * get_part_name(Part p) {return p->name; }
PartQuantity get_part_on_hand(Part p) {return p->on_hand; }
PartPrice get_part_price(Part p)  {return p->price; }
void destroy_part(Part p)
{
  free(p);
}
void print_part(Part p)
{
  printf("%-11d       %-*s   %-11d       %-s\n",
         get_part_number(p),
         NAME_LEN,
         get_part_name(p),
         get_part_on_hand(p),
         dollars(p));
}
/* locale needed for currency formatting */
void init_locale(void)
{
  setlocale(LC_ALL, "en_US");
}
char * dollars(Part p)
{
  static char s[16];
  strfmon(s, sizeof(s) - 1, "%n",  (double)(get_part_price(p) / 100.00));

  return s;
}
