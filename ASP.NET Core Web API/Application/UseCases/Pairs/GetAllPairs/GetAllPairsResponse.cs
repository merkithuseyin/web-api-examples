using System.Collections.Generic;
using Domain.Entities;

namespace Application.UseCases.Pairs.GetAllPairs;

public record GetAllPairsResponse(ICollection<Pair> Pairs);