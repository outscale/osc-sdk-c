/* ANSI C compatible example */

#include "osc_sdk.h"

int main(void)
{
	struct osc_env e;
	struct osc_str r;
	struct osc_read_images_arg arg = {0};
	int ret = 1;
	int curl_res;

	arg.filters_str = "{\"AccountAliases\": [\"Outscale\"]}";
	if (osc_init_sdk(&e, NULL, 0) < 0) {
		fprintf(stderr, "fail to init osc-sdk-c\n");
		return 1;
	}
	osc_init_str(&r);

	curl_res = osc_read_load_balancers(&e, &r, NULL);
	if (curl_res) {
		fprintf(stderr, "fail to read load balancer %s\n",
			curl_easy_strerror(curl_res));
		goto out;
	}
	printf("== osc_read_load_balancers ==\n%s\n", r.buf);
	osc_deinit_str(&r);

	if (osc_read_images(&e, &r, &arg) < 0) {
		fprintf(stderr, "fail to read images\n");
		goto out;
	}
	printf("== osc_read_images ==\n%s\n", r.buf);

	ret = 1;
out:
	osc_deinit_str(&r);
	osc_deinit_sdk(&e);
	return ret;
}
