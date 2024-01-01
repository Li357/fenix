#include "nvic.h"

void nvic_enable_irq(nvic_irq_t irq) {
  NVIC->ISER[IRQINDEX(irq)] |= IRQBIT(irq);
}

void nvic_disable_irq(nvic_irq_t irq) {
  NVIC->ICER[IRQINDEX(irq)] &= ~IRQBIT(irq);
}

void nvic_set_priority(nvic_irq_t irq, uint8_t priority) {
  NVIC->IPR[IRQPRINDEX(irq)] &= ~IRQPRMASK(irq);
  NVIC->IPR[IRQPRINDEX(irq)] |= priority << IRQPRSHIFT(irq);
}
