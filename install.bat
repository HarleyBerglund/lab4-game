git clone --branch v3.1.0 https://github.com/NordicSemiconductor/nrfx.git
git clone --branch 5.9.0 https://github.com/ARM-software/CMSIS_5.git
mkdir nrfx\CMSIS\Core
xcopy /E "CMSIS_5\CMSIS\Core" "nrfx\CMSIS\Core"
rmdir /s /q CMSIS_5
cd nrfx
rmdir /s /q .git
mkdir src\sample
mkdir build
cd ..
copy "nrfx\templates\nrfx_config.h" "nrfx\src\sample\"
copy "nrfx\templates\nrfx_config_nrf5340_application.h" "nrfx\src\sample\"
copy "nrfx\templates\nrfx_config_common.h" "nrfx\src\"
copy "nrfx\templates\nrfx_glue.h" "nrfx\src\"
copy "nrfx\templates\nrfx_log.h" "nrfx\src\"