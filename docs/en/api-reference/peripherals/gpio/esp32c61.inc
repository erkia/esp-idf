.. This file gets included from other .rst files in this folder.
.. It contains target-specific snippets.
.. Comments and '---' lines act as delimiters.
..
.. This is necessary mainly because RST doesn't support substitutions
.. (defined in RST, not in Python) inside code blocks. If that is ever implemented,
.. These code blocks can be moved back to the main .rst files, with target-specific
.. file names being replaced by substitutions.

.. gpio-summary

The {IDF_TARGET_NAME} chip features 30 physical GPIO pins (GPIO0 ~ GPIO29). Each pin can be used as a general-purpose I/O, or to be connected to an internal peripheral signal. Through GPIO matrix and IO MUX, peripheral input signals can be from any IO pins, and peripheral output signals can be routed to any IO pins. Together these modules provide highly configurable I/O. For more details, see *{IDF_TARGET_NAME} Technical Reference Manual* > *IO MUX and GPIO Matrix (GPIO, IO_MUX)* [`PDF <{IDF_TARGET_TRM_EN_URL}#iomuxgpio>`__].

The table below provides more information on pin usage, and please note the comments in the table for GPIOs with restrictions.

.. list-table::
    :header-rows: 1
    :widths: 8 12 12 20

    * - GPIO
      - Analog Function
      - LP GPIO
      - Comments

    * - GPIO0
      -
      - LP_GPIO0
      -

    * - GPIO1
      - ADC1_CH0
      - LP_GPIO1
      -

    * - GPIO2
      -
      - LP_GPIO2
      -

    * - GPIO3
      - ADC1_CH1
      - LP_GPIO3
      -

    * - GPIO4
      - ADC1_CH2
      - LP_GPIO4
      -

    * - GPIO5
      - ADC1_CH3
      - LP_GPIO5
      -

    * - GPIO6
      -
      - LP_GPIO6
      -

    * - GPIO7
      -
      -
      -

    * - GPIO8
      -
      -
      -

    * - GPIO9
      -
      -
      -

    * - GPIO10
      -
      -
      -

    * - GPIO11
      -
      -
      -

    * - GPIO12
      -
      -
      - USB-JTAG

    * - GPIO13
      -
      -
      - USB-JTAG

    * - GPIO14
      -
      -
      - SPI0/1

    * - GPIO15
      -
      -
      - SPI0/1

    * - GPIO16
      -
      -
      - SPI0/1

    * - GPIO17
      -
      -
      - SPI0/1

    * - GPIO18
      -
      -
      -

    * - GPIO19
      -
      -
      - SPI0/1

    * - GPIO20
      -
      -
      - SPI0/1

    * - GPIO21
      -
      -
      - SPI0/1

    * - GPIO22
      -
      -
      -

    * - GPIO23
      -
      -
      -

    * - GPIO24
      -
      -
      -

    * - GPIO25
      -
      -
      -

    * - GPIO26
      -
      -
      -

    * - GPIO27
      -
      -
      -

    * - GPIO28
      -
      -
      -

    * - GPIO29
      -
      -
      -

.. note::

  - Some pins are used as strapping pins, which can be used to select in which boot mode to load the chip, etc.. The details can be found in `datasheet <{IDF_TARGET_DATASHEET_EN_URL}>`_ > ``Strapping Pins``.
	- SPI0/1: GPIO14 ~ GPIO17 and GPIO19 ~ GPIO21 are usually used for SPI flash and PSRAM, they're not recommended for other uses.
	- USB-JTAG: GPIO12 and GPIO13 are used by USB-JTAG by default. If they are reconfigured to operate as normal GPIOs, USB-JTAG functionality will be disabled.

---
