/*
 *  Copyright 2024 Zoé Cassiopée Gauthier
 *
 *  This file is part of Scream.
 *
 *  Scream is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Scream is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Scream.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define SCREAM_MAJOR 0

static loff_t scream_llseek(struct file *, loff_t, int);
static ssize_t scream_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t scream_write(struct file *, const char __user *, size_t,
			    loff_t *);

static const struct file_operations scream_fops = {
    .owner = THIS_MODULE,
    .llseek = scream_llseek,
    .read = scream_read,
    .write = scream_write,
};

static struct cdev scream_cdev;
static int scream_major = 0;

static loff_t scream_llseek(struct file *file, loff_t offset, int orig)
{
	return file->f_pos = 0;
}

static ssize_t scream_read(struct file *file, char __user *buf, size_t count,
			   loff_t *ppos)
{
	char is_capital;

	get_random_bytes(&is_capital, sizeof(is_capital));
	is_capital &= 0xf;

	if (put_user(is_capital == 0 ? 'A' : 'a', buf))
		return -EFAULT;

	return 1;
}

static ssize_t scream_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	return count;
}

static int __init scream_init(void)
{
	dev_t dev;

	if (alloc_chrdev_region(&dev, 0, 1, "scream") < 0)
		panic("Couldn't register /dev/scream driver\n");
	scream_major = MAJOR(dev);

	cdev_init(&scream_cdev, &scream_fops);
	scream_cdev.owner = THIS_MODULE;
	scream_cdev.ops = &scream_fops;
	cdev_add(&scream_cdev, MKDEV(scream_major, 0), 1);

	return 0;
}

static void __exit scream_cleanup(void)
{
	cdev_del(&scream_cdev);
	unregister_chrdev_region(MKDEV(scream_major, 0), 1);
}

module_init(scream_init);
module_exit(scream_cleanup);

MODULE_DESCRIPTION("Character driver to output screaming");
MODULE_VERSION("1.0");
MODULE_LICENSE("GPL");
