
#include "wrapper.h"

DEFINE_PER_CPU(uint64_t, n_hits) = 0;

EXPORT_PER_CPU_SYMBOL(n_hits);

DEFINE_PER_CPU(uint64_t, n_misses) = 0;

EXPORT_PER_CPU_SYMBOL(n_misses);

// TODO decide when to include recording

//DEFINE_SPINLOCK(record_from_label);

//EXPORT_SYMBOL(record_from_label);

//struct address_entry lbr_entries[NUM_ENTRIES]={0};

//EXPORT_SYMBOL(lbr_entries);

//DEFINE_PER_CPU(addr_entry[NUM_ENTRIES], lbr_percpu_entries) = {0};

//EXPORT_PER_CPU_SYMBOL(lbr_percpu_entries);

struct address_entry lbr_entry_table[NUM_UNITS][NUM_ENTRIES] = {0};
EXPORT_SYMBOL(lbr_entry_table);


#ifndef INCLUDE_EXTENDED_MEASUREMENTS
void add_lbr_entry_default(struct address_entry *entry)
{
   int i, cpu;
   struct address_entry *lbr_entries;
   cpu = get_cpu();
   lbr_entries = lbr_entry_table[cpu];
   for (i = 0; i < NUM_ENTRIES; i++)
   {
      if(lbr_entries[i].to == entry->to)
      {
          lbr_entries[i].nhits++;
          break;
      }
      if(lbr_entries[i].to == 0)
      {
           lbr_entries[i].tos = entry->tos;
           lbr_entries[i].from = entry->from;
           lbr_entries[i].to = entry->to;
           lbr_entries[i].address= entry->address;
           lbr_entries[i].nhits = 1;
           break;
      }
   }
   put_cpu();
}
#else
unsigned long long getXXSignature()
{
   return current->xxx_profile;
}

EXPORT_SYMBOL(getXXSignature);

void setXXSignature(unsigned long long value)
{
   current->xxx_profile = value;
}

EXPORT_SYMBOL(setXXSignature);

void add_lbr_entry_default(struct from_entry *entry)
{
   int i, cpu, j;
   struct address_entry *lbr_entries;
   cpu = get_cpu();
   lbr_entries = lbr_entry_table[cpu];
   for (i = 0; i < NUM_ENTRIES; i++)
   {
      if(lbr_entries[i].to == entry->to)
      {
          lbr_entries[i].nhits++;
          for(j = 0; j < lbr_entries[i].from_length; j++)
          {
                if(lbr_entries[i].from[j].from == entry->from)
                {
                     lbr_entries[i].from[j].nhits++;
                     break;
                }
          }
          if ((j ==  lbr_entries[i].from_length) && (lbr_entries[i].from_length < NUM_FROM))
          {
             lbr_entries[i].from_length++;
             lbr_entries[i].from[j].from = entry->from;
             lbr_entries[i].from[j].sig =  entry->sig;
             lbr_entries[i].from[j].nhits = 1;    
          }
           
          break;
      }
      if(lbr_entries[i].to == 0)
      {
           lbr_entries[i].tos = entry->tos;
           lbr_entries[i].from[0].from = entry->from;
           lbr_entries[i].from[0].nhits = 1;
           lbr_entries[i].from[0].sig = entry->sig;
           lbr_entries[i].from_length = 1;
           lbr_entries[i].to = entry->to;
           lbr_entries[i].address= entry->address;
           lbr_entries[i].nhits = 1;
           break;
      }
   }
   put_cpu();
}
#endif

add_lbr_entry_func_t  add_lbr_entry = add_lbr_entry_default;

EXPORT_SYMBOL(add_lbr_entry);












