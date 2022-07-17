using Core.Models;

namespace Business_Logic.Models;

public class Pair: IPair
{
    public int Id { get; set; }
    public string? Name { get; set; }
    public string? Value { get; set; }
}