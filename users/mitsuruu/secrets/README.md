# Secrets
In order to make the secrets work, you'll have to add a file `secrets.h` alongside the existing `secrets.c` and fill it with the following contents, replacing BLANK# with whatever secrets/passwords/whatever you want. Note that the `secrets.h` file won't be committed to Git to protect your secrets, but if you have multiple computers that you use QMK with, then you'll have to replicate the file outside of git. 
```c
static const char *const secret[] = {
    "BLANK1",
    "BLANK2",
    "BLANK3",
    "BLANK4",
    "BLANK5"
};

bool process_record_secrets(uint16_t keycode, keyrecord_t *record);
```

