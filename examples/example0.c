#include "osc_sdk.h"

int main(void)
{
	struct osc_env e;
	struct osc_str r;

	if (osc_init_sdk(&e, NULL, 0) < 0)
		return 1;
	osc_init_str(&r);

	osc_read_load_balancers(&e, &r, NULL);
	printf("[%s", r.buf);
	osc_deinit_str(&r);

	osc_read_images(&e, &r, &(struct osc_read_images_arg)
			{.filters_str="{\"AccountAliases\": [\"Outscale\"]}"}
		);
	printf(",\n%s]\n", r.buf);

	osc_deinit_str(&r);
	osc_deinit_sdk(&e);
	return 0;
}
