#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/utsname.h>
#include <asm/pgtable.h>
#include <linux/string.h>

int page_by_address(struct task_struct *task, const unsigned long addr)
{
    pgd_t *pgd;
    pte_t *ptep, pte;
    pud_t *pud;
    pmd_t *pmd;
    unsigned int accessed = 0;
    struct mm_struct *mm = task->mm;
    pgd = pgd_offset(mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
        goto out;
    pud = pud_offset(pgd, addr);
    if (pud_none(*pud) || pud_bad(*pud))
        goto out;
    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
        goto out;
    ptep = pte_offset_map(pmd, addr);
    if (!ptep)
        goto out;
    pte = *ptep;
    if (pte_young(pte))
	accessed = 1;
    pte = pte_mkold(pte);
    *ptep = pte;
out:
    return accessed;
}


static int mm_exp_load(void)
{
	struct mm_struct *mm;
	struct task_struct *task;
	struct task_struct *task_thread;
	for_each_process(task) 
	{		
		if (strcmp(task->comm,"qemu-system-x86") == 0) 
		{
			printk("-------------------------");
			task_thread = task;
			do {
				mm = task_thread->mm;
				unsigned int page_accessed = 0;
				const struct vm_area_struct *vma = mm->mmap;
 		       			while (vma != NULL) 
					{
    		    	 	  	 	unsigned long address;
        	 	   			for (address = vma->vm_start; address < vma->vm_end; address += PAGE_SIZE) 
						 {
        	  	   			   page_accessed += page_by_address(task_thread, address);
        	  	 			 }
        	  	  		vma = vma->vm_next;
       					}
				printk("\n%s[%d] = %d\n", task_thread->comm, task_thread->pid,page_accessed);
		       }while_each_thread(task, task_thread);
		}
	 }
        return 0;
}

static void mm_exp_unload(void)
{
        printk("\nPrint segment information module exiting.\n");
}

module_init(mm_exp_load);
module_exit(mm_exp_unload);


MODULE_AUTHOR ("Hani Nemati, hani.nemati@gmail.com");
MODULE_DESCRIPTION ("Print memory accessed bit");
MODULE_LICENSE("GPL");
