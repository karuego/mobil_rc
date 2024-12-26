set dotenv-load
set dotenv-required

default:
    echo 'Hello, world!'

push:
    git push --all --tags

build +args='--verify':
    arduino-cli compile {{ args }} -b $FQBN --port $PORT \
        --warnings all --jobs 0 \
        --build-property "build.extra_flags=\
        -DUSE_ENV_VAR \
        -DBLYNK_TEMPLATE_ID=\"$BLYNK_TEMPLATE_ID\" \
        -DBLYNK_TEMPLATE_NAME=\"$BLYNK_TEMPLATE_NAME\" \
        -DBLYNK_AUTH_TOKEN=\"$BLYNK_AUTH_TOKEN\" \
        -DWIFI_SSID=\"$WIFI_SSID\" \
        -DWIFI_PASS=\"$WIFI_PASS\""

upload: (build '--upload')
