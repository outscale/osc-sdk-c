#include "osc_sdk.h"

int main(void)
{
        struct osc_env e;
        struct osc_str r;

        osc_init_sdk(&e, NULL, 0);
        osc_init_str(&r);

        osc_read_load_balancers(&e, &r, NULL);
        printf("[%s", r.buf);
        osc_deinit_str(&r);

        osc_read_images(&e, &r, &(struct osc_read_images_arg)
			{.filters="{\"AccountAliases\": [\"Outscale\"]}"}
		);
        printf(",\n%s]\n", r.buf);

        osc_deinit_str(&r);
        osc_deinit_sdk(&e);
}
