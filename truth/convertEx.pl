@in = <>;
@data;
$patch = 15*15;
while($in[0]=~/^\#/){shift @in;}
for($i=0;$i<$patch;$i++)
{
  chomp;
  $_ = $in[$i];
  s/^\s+//;s/\s+$//;
  @r = split(/ /);
  push(@data,[@r]);
}
$num = scalar @{$data[0]};
print "Number of Examples: $num\n";
for($i=0;$i<$num;$i++)
{
  for($j=0;$j<$patch;$j++)
  {
    printf("%f ",$data[$j][$i]);
  }
  print "\n";
}
