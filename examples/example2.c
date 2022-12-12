#include "osc_sdk.h"
#include <json.h>
#include <string.h>


enum ACTION {
	READ,
	READ_ID,
	STOP_VM,
	START_VM,
	TERMINATE_VM
};

int main(int ac, char **av)
{
	char *vm_ids[1024];
	int action_type = 0;
	char filer_buf[1024];
	_Bool terminate_no_ask = 0;

	if (ac < 2) {
		printf("Usage: %s VM_NAME_FILTER [ACTION]\n", av[0]);
		return 1;
	}

	if (strlen(av[1]) > 1000) {
		printf("VM_NAME_FILTER too long\n");
		return 1;
	}

	if (ac >= 3) {
		if (!strcmp(av[2], "READ_ID"))
			action_type = READ_ID;
		else if (!strcmp(av[2], "STOP_VM"))
			action_type = STOP_VM;
		else if (!strcmp(av[2], "START_VM"))
			action_type = START_VM;
		else if (!strcmp(av[2], "TERMINATE_VM"))
			action_type = TERMINATE_VM;
		else if (strcmp(av[2], "READ")) {
			fprintf(stderr,
				"ACTION must be READ_ID, STOP_VM, START_VM,"
				"TERMINATE_VM or READ\n");
			return 1;
		}
		if (ac > 3 && !strcmp(av[3], "Fire ! Fire ! Fire !"))
			terminate_no_ask = 1;
	}

        auto_osc_env struct osc_env e;
        auto_osc_str struct osc_str r;
	json_object *jobj;
	json_object *vms;

        if (osc_init_sdk(&e, NULL, OSC_VERBOSE_MODE) < 0)
		return 1;
        osc_init_str(&r);

	sprintf(filer_buf, "*%s*", av[1]);
	/* ./cognac ReadVms --Filters.TagKeys '["Name"]' --Filters.TagValues '["*buntu*"]' */
        if (osc_read_vms(&e, &r, &(struct osc_read_vms_arg)
				{.filters=(struct filters_vm) {
						.tag_keys_str = "[\"Name\"]",
						.tag_values = (char *[2]) {
							filer_buf,
							NULL
						}
					},
				 .is_set_filters = 1}
				) < 0)
		return 1;

	/* this is for json pretty print */
	jobj = json_tokener_parse(r.buf);
	vms = json_object_object_get(jobj, "Vms");
	if (!vms) {
		fprintf(stderr, "can't get Vms, ret:\n%s\n",
			r.buf);
	}
	if (action_type == READ) {
		puts(json_object_to_json_string_ext(vms,
						    JSON_C_TO_STRING_PRETTY |
						    JSON_C_TO_STRING_NOSLASHESCAPE));
	} else if (action_type == READ_ID ||
		   action_type == STOP_VM || action_type == START_VM ||
		   action_type == TERMINATE_VM) {
		for (int i = 0; i < json_object_array_length(vms); ++i) {
			json_object *vm = json_object_array_get_idx(vms, i);
			json_object *vm_id = json_object_object_get(vm, "VmId");

			if (action_type == STOP_VM || action_type == START_VM ||
			    action_type == TERMINATE_VM) {
				vm_ids[i] = (char *)json_object_get_string(vm_id);
				vm_ids[i + 1] = NULL;
			}

			if (action_type == READ_ID ||
			    action_type == TERMINATE_VM) {
				// get vm["Tags"][0]["Value"]
				json_object *name = json_object_object_get(vm, "Tags");

				name = name ? json_object_array_get_idx(name, 0) : NULL;
				name = name ? json_object_object_get(name, "Value") : NULL;
				printf("%s: %s\n",
				       name ? json_object_to_json_string_ext(name, 0) : "(nil)",
				       json_object_to_json_string_ext(vm_id, 0));
			}
		}
	}
	if (action_type == STOP_VM) {
		osc_deinit_str(&r);
		osc_stop_vms(&e, &r, &(struct osc_stop_vms_arg) {
				.vm_ids=vm_ids
			});
		printf("%s\n", r.buf);
	} else if (action_type == START_VM) {
		osc_deinit_str(&r);
		osc_start_vms(&e, &r, &(struct osc_start_vms_arg) {
				.vm_ids=vm_ids
			});
		printf("%s\n", r.buf);
	} else if (action_type == TERMINATE_VM) {
		char answer[4];

		printf("Will be TERMINATED (it mean die (it mean destroy))\n");
		if (!terminate_no_ask) {
			printf("ARE YOU SURE ? (yes/No)\n");
			scanf("%3s", answer);
			answer[3] = 0;
		}

		if (terminate_no_ask ||
		    !strcmp("yes", answer)) {
			osc_deinit_str(&r);
			osc_delete_vms(&e, &r, &(struct osc_delete_vms_arg) {
					.vm_ids=vm_ids
				});
			printf("%s\n", r.buf);
		} else {
			printf("Terminate was CANCEL (it mean it wasn't done (So the VM(Virtual Machines)\n"
			       "Are Alive(not destroy (Your not reading lisp BTW(Nor schme(scheme is a variante of lisp(it's a joke because the is a lot of (parentesis) in lisp))))))) !!!\n");
		}
	}

	json_object_put(jobj);
	/* C99 return 0 by default in main */
}
