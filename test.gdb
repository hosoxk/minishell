define printl
  set $token = token_list
  if $token == 0
    printf "token_list is empty or NULL\n"
    return
  end
  while $token != 0
    printf "value: %s, type: %d, is_command: %d, command_type: %d\n", $token->value, $token->type, $token->is_command, $token->command_type
    set $token = $token->next
    if $token == 0
      break
    end
  end
end
