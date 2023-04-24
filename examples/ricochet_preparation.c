/* not an examples per-say
 * a but small code called to prepare ricochet(mock server)
 * in order to create resources so we have similar result at
 * what we have when examples call integration_tests.sh
 */

#include "osc_sdk.h"

int main(void)
{
	auto_osc_env struct osc_env e;
        auto_osc_str struct osc_str r;

	if (osc_init_sdk_ext(&e, NULL, OSC_VERBOSE_MODE, &(struct osc_env_conf){
				.login="11112211111110000000",
				.password="0000001111112222223333334444445555555666"
			}) < 0)
		return 1;
        osc_init_str(&r);

	struct osc_create_image_arg img = {
		.image_name="Arch"
	};

	osc_create_image(&e, &r, &img);
	printf("%s\n", r.buf ? r.buf : "(nil)");
}

