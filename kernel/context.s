.section .text
.global context_switch

# a0 = &old->regs
# a1 = &new->regs

context_switch:
  # Salva contexto atual
  sd ra, 0(a0)
  sd sp, 8(a0)
  sd gp, 16(a0)
  # ... Botar resto dos registradores relevantes
  sd t6, 240(a0)

  # Carrega contexto da prxima task
  ld ra, 0(a1)
  ld sp, 8(a1)
  ld gp, 16(a1)
  # ... Botar resto dos registradores relevantes
  ld t6, 240(a1)

  ret
