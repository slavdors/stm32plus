/*
 * This file is a part of the open source stm32plus library.
 * Copyright (c) 2011,2012,2013 Andy Brown <www.andybrown.me.uk>
 * Please see website for licensing terms.
 */

#pragma once

// ensure the MCU series is correct
#ifndef STM32PLUS_F1
#error This class can only be used with the STM32F1 series
#endif


namespace stm32plus {


  /**
   * Template base class for ADC peripherals.
   *
   * @tparam TPeripheralName The peripheral name (PERIPHERAL_ADC1, PERIPHERAL_ADC2, PERIPHERAL_ADC3)
   */

  template<PeripheralName TPeripheralName>
  class AdcPeripheral : public Adc {

    protected:
      void initialisePeripheral();

    public:
      AdcPeripheral(Adc *master);
      ~AdcPeripheral();
  };


  /**
   * Constructor
   * @param params The parameters class used to initialise the peripheral
   */

  template<PeripheralName TPeripheralName>
  inline AdcPeripheral<TPeripheralName>::AdcPeripheral(Adc *master)
    : Adc((ADC_TypeDef *)PeripheralTraits<TPeripheralName>::PERIPHERAL_BASE,master) {
  }


  /**
   * Called from the derived class constructor after the features have been constructed
   * and performed any customisation of the init and commoninit values.
   */

  template<PeripheralName TPeripheralName>
  inline void AdcPeripheral<TPeripheralName>::initialisePeripheral() {

    // clocks on first

    ClockControl<TPeripheralName>::On();

    // disable and deinit

    ADC_Cmd(_peripheralAddress,DISABLE);
    ADC_DeInit(_peripheralAddress);

    // if in dual mode and this is the slave then copy over the mode

    if(_master)
      _init.ADC_Mode=static_cast<ADC_InitTypeDef *>(*_master)->ADC_Mode;

    // the features have been constructed and any customisations have been
    // made to the init structure. Initialise and free the memory it was using

    ADC_Init(_peripheralAddress,&_init);

    // if there are any injected channels then set the count

    if(_injectedChannelCount)
      ADC_InjectedSequencerLengthConfig(_peripheralAddress,_injectedChannelCount);
  }


  /**
   * Destructor, disable peripheral and disable clocks
   */

  template<PeripheralName TPeripheralName>
  inline AdcPeripheral<TPeripheralName>::~AdcPeripheral() {

    // disable the peripheral

    disablePeripheral();

    // clocks off

    ClockControl<TPeripheralName>::Off();
  }
}
