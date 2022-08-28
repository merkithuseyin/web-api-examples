using Domain.Common;

namespace Domain.Entities;

public class Pair: BaseEntity<int>
{
    public string? Name { get; set; }
    public string? Value { get; set; }
}