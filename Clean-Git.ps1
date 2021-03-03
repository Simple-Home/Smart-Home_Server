function Remove-MergedBranches
{
  git branch --merged |
    ForEach-Object { $_.Trim() } |
    Where-Object {$_ -NotMatch "^\*"} |
    Where-Object {-not ( $_ -Like "*master" )} |
    ForEach-Object { git branch -d $_ }
}