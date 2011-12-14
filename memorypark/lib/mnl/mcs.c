void mcs_hdf_escape_val(HDF *hdf)
{
    char *esc = NULL, *val = NULL;
    HDF *child, *next;
    
    if (!hdf) return;

    val = hdf_obj_value(hdf);
    if (val) {
        if (mutil_real_escape_string_nalloc(&esc, val, strlen(val))) {
            hdf_set_value(hdf, NULL, esc);
            free(esc);
        }
    }

    child = hdf_obj_child(hdf);
    if (child) {
        mcs_hdf_escape_val(child);
    }

    next = hdf_obj_next(hdf);
    while (next) {
        mcs_hdf_escape_val(next);
        next = hdf_obj_next(next);
    }
}
