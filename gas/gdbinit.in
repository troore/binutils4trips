dir .

set listsize 25

# break md_assemble
# break tc_gen_reloc
# break bfd_install_relocation
break trips_set_fix
# break md_apply_fix3
# break trips_parse_field_operator
# break _trips_capture_any_node
# break _trips_place_instruction_in_grid
# break _trips_capture_specific_alu_node
# break _trips_build_the_instruction
# break _close_block_specifier
# break _close_vliw_specifier
# break _make_block_specifier
# break _make_vliw_specifier
break _trips_flush_grid
# break   bfd_elf32_write_shdrs_and_ehdr
# break   trips_elf_post_process_headers
# break   trips_mach
# vvvvvvvvvv  Next is not so useful vvvvvvvvvvvv
# break   trips_arch
# break _trips_flush_registers
# break _trips_pseudo_op_sequence
break load_hw_config_file

#  remove all these breakpoints
# break as_warn
# break as_warn_where
# break as_bad
# break as_bad_where
# break as_fatal
# break as_perror
# break as_assert
# break as_abort

define pe
call print_expr ($)
end

document pe
Print *$ as an expressionS, expanding parameters.
end

define ps
call print_symbol_value ($)
end

document ps
Print *$ as a symbolS, including expression value.
end

define pf
call print_fixup ($)
end

document pf
Print *$ as a fixS, including symbol value.
end

# Put this last, in case it fails.

# break abort
