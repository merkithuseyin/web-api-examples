using Application.Interfaces;
using MediatR;

namespace Application.UseCases.Pairs.GetAllPairs;

public record GetAllPairsRequest: IRequest<GetAllPairsResponse>, IRequestValidator
{
    public void Validate()
    {
    }
}