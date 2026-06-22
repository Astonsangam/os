param (
    [switch]$run
)

$NASM = "C:\Users\Dell\AppData\Local\bin\NASM\nasm.exe"
$GXX = "g++"
$LD = "ld"
$OBJCOPY = "objcopy"
$QEMU = "C:\Program Files\qemu\qemu-system-i386.exe"

# Ensure directories exist
if (!(Test-Path src/boot)) { exit 1 }

# Build assembly
$ASM_FILES = Get-ChildItem src/boot/*.s
foreach ($file in $ASM_FILES) {
    $obj = $file.FullName.Replace(".s", ".o")
    Write-Host "Assembling $($file.Name)..."
    & $NASM -f win32 --prefix _ $file.FullName -o $obj
}

# Build C++
$CPP_FILES = Get-ChildItem src/kernel/*.cpp
foreach ($file in $CPP_FILES) {
    $obj = $file.FullName.Replace(".cpp", ".o")
    Write-Host "Compiling $($file.Name)..."
    & $GXX -c -m32 -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -Iinclude $file.FullName -o $obj
}

# Link
$BOOT_OBJS = Get-ChildItem src/boot/*.o | ForEach-Object { $_.FullName }
$KERNEL_OBJS = Get-ChildItem src/kernel/*.o | ForEach-Object { $_.FullName }
$OBJS = $BOOT_OBJS + $KERNEL_OBJS
Write-Host "Linking kernel.pe..."
& $LD -mi386pe -T linker.ld -nostdlib -o kernel.pe $OBJS
Write-Host "Converting kernel.pe to ELF kernel.bin..."
& $OBJCOPY -O elf32-i386 kernel.pe kernel.bin

if (Test-Path kernel.bin) {
    Write-Host "Build Successful: kernel.bin created."
    if ($run) {
        Write-Host "Starting QEMU..."
        & $QEMU -kernel kernel.bin
    }
} else {
    Write-Host "Build Failed."
}
