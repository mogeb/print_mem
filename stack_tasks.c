#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/utsname.h>

static void print_mem(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
//	struct pt_regs *regs = task_pt_regs(task);
//	unsigned long sp = kernel_stack_pointer(regs);
//	unsigned long cr3 = read_cr3(); 
        mm = task->mm;
	struct mm_struct *mm_active = task->active_mm;
	//unsigned long total_vm_t = mm.total_vm;
        unsigned long mem_total = 0; 
        unsigned long mem_active_total = 0; 	
        for (vma = mm->mmap ; vma ; vma = vma->vm_next) {
		//printk("Mapped Memory = 0x%lx,0x%lx\n",vma->vm_start,vma->vm_end);
		//if (mm->start_stack >= vma->vm_start && mm->start_stack <= vma->vm_end){
		//if (regs->sp >= vma->vm_start && regs->sp <= vma->vm_end){	
			
		//	printk("0x%lx,0x%lx,0x%lx,0x%lx,%s,%d,%s,0x%lx,0x%lx\n", regs->sp,mm->start_stack ,vma->vm_start, vma->vm_end, task->comm, task->pid, utsname()->nodename, task->thread.sp0, cr3);
			
		//}
		mem_total += vma->vm_end - vma->vm_start;
        }
	for (vma = mm_active->mmap; vma ; vma = vma->vm_next) { 
		//printk("Active Memory = 0x%lx,0x%lx\n",vma->vm_start,vma->vm_end);
		mem_active_total += vma->vm_end - vma->vm_start;
	}
        //printk("Stack Segment start = 0x%lx\n" , mm->start_stack);
	printk("mem_mapped = %lx, mem_active= %lx\n",mem_total,mem_active_total);
}

static int mm_exp_load(void){
        struct task_struct *task;

        for_each_process(task) {
		if (task->comm != NULL && task->mm != NULL) {
                        print_mem(task);
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


MODULE_AUTHOR ("Krishnakumar. R, rkrishnakumar@gmail.com");
MODULE_DESCRIPTION ("Print segment information");
MODULE_LICENSE("GPL");
