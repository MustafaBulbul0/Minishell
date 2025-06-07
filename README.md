### 🎯 Project Overview

This project aims to develop a simple shell application that mimics basic UNIX shell functionalities. The goal is to create a shell that can parse and execute user commands, supporting features like input/output redirection and piping.

### ✅ Required Features

- The program must operate as a standalone terminal application.
- Upon launch, it should display a prompt (e.g., `minishell$`).
- User input should be correctly parsed and executed.
- The following built-in commands must be supported:
  - `echo` (with `-n` support)
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- External commands (e.g., `ls`, `cat`, `grep`) should be executable using the `PATH` variable.
- Pipes (`|`) must be handled to pass output from one command to another.
- Input and output redirections must be supported:
  - `<` input redirection
  - `>` output redirection
  - `>>` append output redirection
  - `<<` here-document (heredoc)
- Environment variable expansion (e.g., `$HOME`, `$USER`) must be correctly implemented.
- Quoting must be handled according to shell rules:
  - Single quotes `'` prevent all expansions.
  - Double quotes `"` allow variable expansion but prevent word splitting.
- Signals like `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` must be properly managed.
- There must be no memory leaks (testable using tools like `valgrind`).
	> ❗While memory leaks caused by the readline() function are acceptable and not your responsibility to fix, this does not give you permission to let your own code leak memory.
- Using `readline` and `history` is encouraged but not mandatory.

------------

### 🎯 Proje Tanımı

Bu proje, temel UNIX kabuk işlevlerini taklit eden basit bir kabuk (shell) uygulaması geliştirmeyi amaçlar. Amaç, kullanıcıdan alınan komutları analiz edip çalıştırabilen, giriş/çıkış yönlendirmeleri ve boru (pipe) işlemleri gibi temel özellikleri destekleyen bir kabuk oluşturmaktır.

### ✅ Zorunlu Özellikler

- Program, kendi başına çalışan bir terminal uygulaması olmalıdır.
- Her başlatıldığında bir istem (örneğin, `minishell$`) göstermelidir.
- Kullanıcı tarafından girilen komutlar doğru bir şekilde ayrıştırılmalı ve yürütülmelidir.
- Aşağıdaki yerleşik (built-in) komutlar desteklenmelidir:
  - `echo` (`-n` desteğiyle)
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Dış komutlar (`ls`, `cat`, `grep` vb.) `PATH` değişkeni kullanılarak çalıştırılabilmelidir.
- Boru (pipe) operatörü (`|`) ile komutlar arasında veri aktarımı yapılabilmelidir.
- Giriş/çıkış yönlendirmeleri desteklenmelidir:
  - `<` giriş yönlendirmesi
  - `>` çıkış yönlendirmesi
  - `>>` ekleme modunda çıkış yönlendirmesi
  - `<<` here-document (heredoc)
- Ortam değişkenlerinin genişletilmesi (`$HOME`, `$USER` vb.) doğru şekilde uygulanmalıdır.
- Tek tırnak (`'`) ve çift tırnak (`"`) kullanımı, kabuk kurallarına uygun şekilde analiz edilmelidir:
  - Tek tırnak içindeki metinler genişletilmez.
  - Çift tırnak içindeki metinlerde değişken genişletmesi yapılır, ancak kelime ayrımı yapılmaz.
- `Ctrl-C`, `Ctrl-D` ve `Ctrl-\` gibi sinyaller uygun şekilde yönetilmelidir.
- Bellek sızıntısı olmamalıdır (`valgrind` gibi araçlarla test edilebilir).
	> ❗readline() fonksiyonunun neden olduğu bellek sızıntılarını düzeltmek zorunda değilsiniz. Ancak bu durum, kendi yazdığınız kodda bellek sızıntılarına izin verebileceğiniz anlamına gelmez.
- `readline` ve `history` kullanımı önerilir ancak zorunlu değildir.
