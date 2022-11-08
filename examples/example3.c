#include "osc_sdk.h"
#include <json.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

/* this example make a VM with a BIG (it mean not small) volume */

char *random_vm_name[] = {
	"RX-78", "MK-II", "Zeta", "Double-Zeta",
	"Nu", "F91", "V",
	"Shining", "God", "Nether", "Rose", "Nobel",
	"Wing",
	"X", "Double-X",
	"Strike", "Strike Rouge", "Freedom", "Justice",
	"AGE",
	"Lupus", "Lupus-Rex",
	"Turn-X", "Turn-A"
};

#define NB_RANDOM_VM_NAME (sizeof random_vm_name / sizeof *random_vm_name)

int usage(const char *prog_name, int ret)
{
	fprintf(ret ? stderr: stdout,
		"Usage: %s [OPTION]\n"
		"	-h			help"
		"	-n	VM_NAME		vm name\n"
		"	-s	VOLUME_SIZE	a BIG volume size", prog_name);
	return ret;
}

int main(int ac, char **av)
{
	char *name;
	int size = 128;
	char *image_id = "ami-f2710ca8";
	auto_osc_env struct osc_env e = {};
	auto_osc_str struct osc_str out = {};
	char *vm_id = NULL;
	json_object *jobj;

	srand(time(0));
	name = random_vm_name[rand() % NB_RANDOM_VM_NAME];

	for (int i = 1; i < ac; ++i) {
		if (!strcmp(av[i], "-n")) {
			if (ac == i + 1) {
				fprintf(stderr, "missing a name");
				return usage(av[0], 1);
			}
			name = av[++i];
		}
		if (!strcmp(av[i], "-s")) {
			if (ac == i + 1) {
				fprintf(stderr, "missing a size");
				return usage(av[0], 1);
			}
			size = atoi(av[++i]);
			if (!size) {
				fprintf(stderr, "Size must be BIG (> 0)");
				return usage(av[0], 1);
			}
		}
		if (!strcmp(av[i], "-h")) {
			return usage(av[0], 0);
		}
	}
	printf("will create vm name %s, with a volume size of %d\n",
	       name, size);

	if (osc_init_sdk(&e, NULL, OSC_VERBOSE_MODE) < 0)
		return 1;
        osc_init_str(&out);

	if (osc_create_vms(&e, &out, &(struct osc_create_vms_arg) {
				.image_id=image_id,
				.block_device_mappings=(struct block_device_mapping_vm_creation[]) {
					{
						.is_set_bsu = 1,
						.bsu=(struct bsu_to_create){
							.is_set_volume_size=1,
							.volume_size=100
						},
						.device_name="/dev/sda1"
					}
				},
				.nb_block_device_mappings=1
			}) < 0) {
		/* osc_deinit_str(&out); */

		/*  /\* try to destroy volume *\/ */
		/* osc_delete_volume(&e, &out, &(struct osc_delete_volume_arg) { */
		/* 		.volume_id=volume_id */
		/* 	}); */
		fprintf(stderr, "ouin ouin, can't make VMs\n");
		return 1;
	}
	jobj = json_tokener_parse(out.buf);
	json_object *vol = json_object_object_get(jobj, "Vms");

	if (!vol) {
		fprintf(stderr, "can't get Vms, ret:\n%s\n",
			out.buf);
	}

	vol = json_object_array_get_idx(vol, 0);
	printf("vol: %p\n", vol);
	vol = json_object_object_get(vol, "VmId");
	vm_id = (char *)json_object_get_string(vol);
	printf("vm made %s\n", vm_id);
	if (!vm_id) {
		fprintf(stderr, "ouin ouin, can't retrive VM ID\n%s\n",
			out.buf);
		return 1;
	}

	osc_deinit_str(&out);
	osc_create_tags(&e, &out, &(struct osc_create_tags_arg) {
			.resource_ids=(char *[]){vm_id, 0},
			.tags=(struct resource_tag[]) {
				{
					.key="Name",
					.value=name
				}
			},
			.nb_tags=1
		});
	printf("DONE Create Vm '%s', tag '%s' creation ret: '%s'\n", vm_id, name, out.buf);
}
