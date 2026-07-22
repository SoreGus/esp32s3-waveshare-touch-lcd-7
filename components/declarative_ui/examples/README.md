# Exemplos reativos

Cada grupo tem uma responsabilidade clara e pode ser ativado em `main/main.cpp`.
Deixe apenas uma chamada `UI::run(...)` descomentada por vez.

- `state/`: estado local de uma tela.
- `binding/`: transformação de um estado para propriedades visuais.
- `state_object/`: Views, ViewModels e suas implementações separados; as propriedades do modelo usam `Published` e as ações ficam nos métodos do ViewModel.
- `list/`: operações mutáveis em `ListState`.
- `for_each/`: conteúdo visual criado a partir de uma coleção observável.
- `compose/`: uma tela completa que combina os recursos reativos e separa ViewModel, componentes e View.
- `showcase/`: aplicativo offline com navegação, imagem embutida, controles e listas; combina as APIs da biblioteca em diferentes Views e ViewModels.

Todas as telas usam o layout compartilhado em `shared/example_layout.hpp`: fundo preenchido, cabeçalho fixo e uma área de conteúdo rolável. Isso permite que o mesmo exemplo se adapte à área disponível da tela sem cortar listas maiores.
