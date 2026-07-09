# =======================================================
# SCRIPT DE EXECUÇÃO EM LOTE - PCSTP SOLVER
# =======================================================

$exe = ".\build\pcstp_solver.exe"
$pastaBase = ".\instances"


$subpastas = @("PCSPG-JMP", "PCSPG-CRR") 

$qtdPorPasta = 15 

$alpha = "0.3"
$iteracoes = "500"
$tamanhoBloco = "50"

Write-Host "Iniciando a bateria de testes..." -ForegroundColor Green

foreach ($pasta in $subpastas) {
    # Ajuste seguro para caminhos
    if ($pasta -eq "") {
        $caminhoBusca = $pastaBase
    } else {
        $caminhoBusca = Join-Path $pastaBase $pasta
    }
    
    Write-Host "Procurando arquivos em: $caminhoBusca" -ForegroundColor DarkGray
    
    if (Test-Path $caminhoBusca) {
        
        # Pega os arquivos garantindo que é um array
        $instancias = @(Get-ChildItem -Path $caminhoBusca -Filter "*.stp" -File) | Select-Object -First $qtdPorPasta
        
        if ($instancias.Count -eq 0) {
            Write-Host "  -> AVISO: A pasta existe, mas nao tem nenhum arquivo '.stp' dentro dela." -ForegroundColor DarkYellow
        }
        
        foreach ($arq in $instancias) {
            $caminhoStp = $arq.FullName
            Write-Host "Testando Instancia: $($arq.Name)" -ForegroundColor Yellow
            
            # Executa o Reativo
            & $exe $caminhoStp "reativo" $iteracoes $tamanhoBloco
        }
    } else {
        Write-Host "  -> ERRO: A pasta '$caminhoBusca' nao foi encontrada." -ForegroundColor Red
    }
}

Write-Host "Bateria de testes finalizada!" -ForegroundColor Green