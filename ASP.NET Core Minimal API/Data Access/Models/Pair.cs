using Core.Models;

namespace Data_Access.Models;

public class Pair: IPair
{
    public int Id { get; set; }
    public string? Name { get; set; }
    public string? Value { get; set; }
}