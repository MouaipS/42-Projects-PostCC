section .text
extern malloc
global ft_strdup ; char *strdup(const char *s) rdi

ft_strdup:
    test    rdi, rdi
    je      .ret_null
    mov     rsi, rdi  

    mov		rcx, 0
    mov     rax, rsi 
.len_loop:
    cmp     byte [rax], 0
    je      .len_done
    inc     rcx
    inc     rax
    jmp     .len_loop

.len_done:
    mov     rdi, rcx   
    inc     rdi             
    sub     rsp, 8          ; aligner la pile à 16 octets avant l'appel
    call    malloc
    add     rsp, 8
    test    rax, rax
    je      .ret_null

    ; rax = dest
    mov     rdx, rax        ; rdx := dest (on le sauvegarde pour retour)
    mov     rdi, rax        ; destination pour rep movsb
    ; rsi contient déjà src
    inc     rcx             ; rcx = len + 1 (inclut le '\0')
    cld                     ; clear direction flag (sens avant)
    rep     movsb           ; copie rcx octets de [rsi] -> [rdi]

    mov     rax, rdx        ; retour : dest
    ret

.ret_null:
    mov rax, 0 ; xor rax, rax
    ret
