#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/pm/device.h> // Required for DPD/Power management
LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

static const struct gpio_dt_spec de = GPIO_DT_SPEC_GET(DT_ALIAS(de), gpios);
static const struct gpio_dt_spec nre = GPIO_DT_SPEC_GET(DT_ALIAS(nre), gpios);

// static struct gpio_dt_spec g_led = GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
static struct gpio_dt_spec g_led = GPIO_DT_SPEC_GET(DT_ALIAS(user_led), gpios);

void SerialSleep()
{
	// Set nRE to 1 and DE to 0 to enter Shutdown mode.
	// https://www.analog.com/media/en/technical-documentation/data-sheets/1481fa.pdf

	int DEReady = gpio_is_ready_dt(&de);
	int nREReady = gpio_is_ready_dt(&nre);

	if (DEReady && nREReady) {

		gpio_pin_set_dt(&de, 0);
		gpio_pin_set_dt(&nre, 1);
	} else {
		LOG_ERR("One of DE/nRE GPIOs not ready!");
	}
}

void SerialWake()
{
	// Set nRE to 1 and DE to 0 to enter Shutdown mode.
	// https://www.analog.com/media/en/technical-documentation/data-sheets/1481fa.pdf

	int DEReady = gpio_is_ready_dt(&de);
	int nREReady = gpio_is_ready_dt(&nre);
	if (DEReady && nREReady) {

		gpio_pin_set_dt(&de, 1);
		gpio_pin_set_dt(&nre, 1);
	} else {
		LOG_ERR("One of DE/nRE GPIOs not ready!");
	}
}

void main(void)
{
	int err;
	if (!gpio_is_ready_dt(&de)) {
		LOG_ERR("The RS485-DE GPIO port is not ready.");
		return 0;
	}

	if (!gpio_is_ready_dt(&nre)) {
		LOG_ERR("The RS485-nRE GPIO port is not ready.");
		return 0;
	}

	if (!gpio_is_ready_dt(&g_led)) {
		LOG_ERR("The LED port is not ready.");
		return 0;
	}

	err = gpio_pin_configure_dt(&de, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		LOG_ERR("Configuring RS485DE GPIO pin failed: %d", err);
		return 0;
	}

	err = gpio_pin_configure_dt(&nre, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		LOG_ERR("Configuring RS485nRE GPIO pin failed: %d", err);
		return 0;
	}

	err = gpio_pin_configure_dt(&g_led, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		LOG_ERR("Configuring LED GPIO pin failed: %d", err);
		return 0;
	}

	SerialWake();

	for (int i = 0; i < 10; i++) {
		LOG_INF("booting: %d", i);
		k_busy_wait(1000 * 200);
	}

	for (int i = 0; i < 2; i++) {
		LOG_INF("hi - busy");
		gpio_pin_set_dt(&g_led, 1);
		k_busy_wait(1000 * 100);
		LOG_INF("lo - busy");
		gpio_pin_set_dt(&g_led, 0);
		k_busy_wait(1000 * 900);
	}
	printk("moving to real sleeps!\n");
	SerialSleep();
	for (int i = 0; i < 10; i++) {
		LOG_INF("hi - sleep");
		gpio_pin_set_dt(&g_led, 1);
		k_sleep(K_MSEC(100));
		LOG_INF("lo - sleep");
		gpio_pin_set_dt(&g_led, 0);
		k_sleep(K_MSEC(900));
	}

	for (;;) {
		k_sleep(K_MSEC(1000));
	}
}
