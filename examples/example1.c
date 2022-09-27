/* GNU C99 example */

#include "osc_sdk.h"
#include <json.h>

int main(void)
{
	/* auto_osc_env and auto_osc_str auto call deinit functions */
        auto_osc_env struct osc_env e;
        auto_osc_str struct osc_str r;
	json_object *jobj;

        if (osc_init_sdk(&e, NULL, 0) < 0)
		return 1;
        osc_init_str(&r);

	/* use C99 compound literal to construct arguments */
        if (osc_read_images(&e, &r, &(struct osc_read_images_arg)
			    {.filters=(struct filters_image) {
					    .account_aliases = "[\"Outscale\"]"
				    }}
		    ) < 0)
		return 1;

	/* this is for json pretty print */
	jobj = json_tokener_parse(r.buf);
	puts(json_object_to_json_string_ext(jobj,
					    JSON_C_TO_STRING_PRETTY |
					    JSON_C_TO_STRING_NOSLASHESCAPE));
	json_object_put(jobj);
	/* C99 return 0 by default in main */
}
